# Tail Call Optimization in Simpl

## Table of Contents

1. [Motivation](#1-motivation)
2. [Background: the problem with naive recursion](#2-background-the-problem-with-naive-recursion)
3. [Approach: trampoline with a `TailCall` sentinel](#3-approach-trampoline-with-a-tailcall-sentinel)
4. [The three mechanisms](#4-the-three-mechanisms)
   - 4.1 [`tail_position_` — tracking where we are](#41-tail_position_--tracking-where-we-are)
   - 4.2 [`TailCall` — the deferred-call sentinel](#42-tailcall--the-deferred-call-sentinel)
   - 4.3 [The trampoline loop](#43-the-trampoline-loop)
5. [How each mechanism connects](#5-how-each-mechanism-connects)
6. [End-to-end trace: `countdown(3)`](#6-end-to-end-trace-countdown3)
7. [Tail-position propagation through `if` and `let`](#7-tail-position-propagation-through-if-and-let)
   - 7.1 [`if`](#71-if)
   - 7.2 [`let`](#72-let)
8. [The callable class hierarchy and why only `UserFn` is trampolined](#8-the-callable-class-hierarchy-and-why-only-userfn-is-trampolined)
9. [What does NOT get TCO and why](#9-what-does-not-get-tco-and-why)
   - 9.1 [Strict built-ins (`*`, `+`, `eval`, `head`, …)](#91-strict-built-ins----eval-head-)
   - 9.2 [`and` and `or`](#92-and-and-or)
   - 9.3 [`do`](#93-do)
   - 9.4 [Lazy user functions (`lazy-fn`)](#94-lazy-user-functions-lazy-fn)
   - 9.5 [Non-self mutual recursion across `eval`](#95-non-self-mutual-recursion-across-eval)
10. [C++ call-stack depth analysis](#10-c-call-stack-depth-analysis)
11. [Modified files reference](#11-modified-files-reference)
12. [Extending TCO to new callables](#12-extending-tco-to-new-callables)

---

## 1. Motivation

Without tail call optimization, a straightforward tail-recursive Simpl function:

```lisp
(defn countdown [n]
  (if (= n 0) 0 (countdown (- n 1))))

(countdown 1000000)
```

builds a C++ call stack 1,000,000+ frames deep — one frame per recursive call —
and crashes with a stack overflow well before reaching the base case.

The goal of TCO is to make tail-recursive Simpl functions run in **O(1) C++ stack
depth** regardless of the recursion depth, enabling idiomatic functional-style
recursion at arbitrary scale.

---

## 2. Background: the problem with naive recursion

The Simpl interpreter evaluates expressions by calling `Interpreter::Evaluate`,
which dispatches via `std::visit` to `EvalVisitor`. When evaluating a function call
`(f arg1 arg2)`:

1. `EvalVisitor::operator()(List&&)` looks up `f`, evaluates the args, and calls
   `callable->Call(interpreter, args)`.
2. `Call` dispatches to `FnCall` in `UserFn`, which creates a new environment,
   binds parameters, and calls `interpreter->Evaluate(body)` — recursing into step 1.

Each level of recursion is a C++ stack frame. For a tail-recursive function with
N recursive calls, this is N nested C++ frames.

---

## 3. Approach: trampoline with a `TailCall` sentinel

The implementation uses the **trampoline** pattern:

- When the interpreter detects a user-function call in **tail position**, instead of
  recursing into C++, it **returns a `TailCall` sentinel** containing the function
  and its already-evaluated arguments.
- The **trampoline loop** — a `while` loop at the first non-tail call site — picks
  up the `TailCall`, invokes the next iteration by calling `CallEvaluated`, and
  repeats until it gets a real value.

The key invariant: **at any point in time, only one frame per trampoline loop
iteration is live on the C++ stack**, regardless of how many logical Simpl
recursions have happened.

---

## 4. The three mechanisms

### 4.1 `tail_position_` — tracking where we are

```cpp
// simpl/interpreter.hh
bool tail_position_ = false;
bool tail_position() const { return tail_position_; }
void set_tail_position(bool v) { tail_position_ = v; }
```

`tail_position_` is a boolean flag on `Interpreter` that answers the question:
*"is the expression I am about to evaluate the last thing this function body needs
to compute?"*

The flag is set by `EvaluateBody` just before evaluating the **last** expression
in a function body (see §4 below), and cleared by `EvalVisitor::operator()(List&&)`
at the start of each function-call evaluation via `std::exchange`:

```cpp
// simpl/interpreter.cc — EvalVisitor::operator()(List&&)
bool in_tail = std::exchange(interpreter->tail_position_, false);
```

`std::exchange` atomically reads the current value and resets it to `false`,
so the flag is always clear by the time any sub-expression is evaluated. Only the
path from `EvaluateBody` through the last expression deliberately re-arms it.

### 4.2 `TailCall` — the deferred-call sentinel

```cpp
// simpl/ast.hh
struct TailCall {
  callable_ptr_t callable;
  ExprList args;  // Already evaluated
  bool operator==(const TailCall&) const = default;
};
```

`TailCall` is a new alternative in the `ExprBase` variant alongside `int_type`,
`std::string`, `List`, etc. It is a pure **implementation artifact** — Simpl
programs never produce or observe a `TailCall` value directly.

When `EvalVisitor` determines that a non-lazy `UserFn` is being called in tail
position, it:

1. Evaluates all arguments eagerly (while the current environment is still live).
2. Returns `TailCall{callable, evaluated_args}` instead of recursing.

The `TailCall` value then **bubbles up** through the return chain until it reaches
the trampoline loop.

A pass-through handler ensures `TailCall` is never accidentally consumed by the
generic `Evaluate` machinery:

```cpp
Expr operator()(TailCall&& tc) { return std::move(tc); }
```

### 4.3 The trampoline loop

The trampoline lives in `EvalVisitor::operator()(List&&)`, at the **non-tail call
site** — i.e., the outermost call that started the whole recursion from a
non-tail context (a top-level expression, a `let` binding, an argument to `*`, etc.):

```cpp
// Non-tail context: run trampoline loop until a real value is returned.
if (!in_tail) {
  while (auto* tc = std::get_if<TailCall>(&call_result)) {
    auto tc_fn = std::dynamic_pointer_cast<Function>(tc->callable);
    if (tc_fn) {
      call_result =
          tc_fn->CallEvaluated(interpreter, std::move(tc->args));
    } else {
      call_result = tc->callable->Call(interpreter, std::move(tc->args));
    }
  }
}
return call_result;
```

`CallEvaluated` is a new method on `Function` that bypasses the normal argument
evaluation step (since arguments are already evaluated and stored in the
`TailCall`):

```cpp
// simpl/function.hh
Expr CallEvaluated(Interpreter* interpreter, args_type&& args) {
  return FnCall(interpreter, std::move(args));
}
```

Each iteration of the `while` loop reuses the **same C++ stack frame**.

---

## 5. How each mechanism connects

```
Interpreter::EvaluateBody(body, env)
│
├─ [non-last expressions] — evaluates normally, tail_position_=false
│
└─ [last expression] — sets tail_position_=true, then evaluates:
   │
   └─ Interpreter::Evaluate(last_expr)
      │
      └─ EvalVisitor::operator()(List&&)  ← e.g. (if (= n 0) 0 (f (- n 1)))
         │   in_tail = exchange(tail_position_, false)  ← reads true, clears
         │
         ├─ if callable is If → restore tail_position_=true, call If::Call
         │     └─ If::Call reads tail_position_, clears for condition,
         │        restores for chosen branch, evaluates branch:
         │           └─ EvalVisitor(List{f, (- n 1)})
         │                in_tail=true, f is UserFn, not lazy
         │                → evaluate args → return TailCall{f, {n-1}}
         │                  ↑ bubbles up through If::Call, EvaluateBody,
         │                    FnCall, Function::Call back to trampoline
         │
         └─ trampoline (in_tail=false at the original call site):
               while call_result is TailCall:
                 call_result = fn->CallEvaluated(interpreter, args)
                 ↑ same C++ stack depth, O(N) iterations
```

---

## 6. End-to-end trace: `countdown(3)`

```lisp
(defn countdown [n]
  (if (= n 0) 0 (countdown (- n 1))))
(countdown 3)
```

**Frame 1 — trampoline owner** (from top-level evaluation of `(countdown 3)`):

1. `EvalVisitor(List{countdown, 3})`: `in_tail=false` (top level).
2. `countdown` is a `UserFn`. `in_tail` is `false`, so no `TailCall` yet.
3. Calls `countdown.Call(interpreter, {3})`.
4. `Function::Call` evaluates arg `3` → `3`. Calls `FnCall({3})`.
5. `UserFn::FnCall`: binds `n=3`, calls `EvaluateBody(body, env)`.
6. `EvaluateBody`: single expression `(if ...)` is last → sets `tail_position_=true`.
7. `Evaluate((if (= n 0) 0 (countdown (- n 1))))`:
   - `EvalVisitor(List{if, ...})`: `in_tail = exchange(true, false) = true`.
   - `if` is `built_in::If` → restores `tail_position_=true`.
   - `If::Call`: `was_tail=true`, clears for condition.
   - Evaluates `(= n 0)` → `false`.
   - Restores `tail_position_=true` for branch.
   - Branch is `(countdown (- n 1))`.
   - `EvalVisitor(List{countdown, (- n 1)})`: `in_tail = exchange(true, false) = true`.
   - `countdown` is `UserFn`, `in_tail=true`, not lazy → evaluate args: `(- n 1)` → `2`.
   - Returns `TailCall{countdown, {2}}`.
8. `TailCall{countdown, {2}}` propagates back to frame 1's trampoline.

**Trampoline iteration 1** (still in frame 1):

9. `tc = &TailCall{countdown, {2}}`.
10. `countdown.CallEvaluated(interpreter, {2})` → `FnCall({2})`.
11. `UserFn::FnCall`: binds `n=2`, calls `EvaluateBody(body, env)`.
12. Body evaluates `(if ...)` with `tail_position_=true` → returns `TailCall{countdown, {1}}`.

**Trampoline iteration 2**:

13. `countdown.CallEvaluated(interpreter, {1})` → ... → `TailCall{countdown, {0}}`.

**Trampoline iteration 3**:

14. `countdown.CallEvaluated(interpreter, {0})`.
15. `EvaluateBody`: sets `tail_position_=true`, evaluates `(if (= n 0) 0 ...)`.
16. `(= n 0)` → `true`. Branch is `0`.
17. `EvalVisitor(int_type{0})`: `in_tail=true` but `0` is not a `List`, handled by
    `operator()(auto&& expr)` → returns `Expr{0}`. No `TailCall`.
18. `EvaluateBody` resets `tail_position_=false`, returns `Expr{0}`.
19. `FnCall` returns `Expr{0}`. `CallEvaluated` returns `Expr{0}`.
20. Trampoline: `std::get_if<TailCall>(&Expr{0})` → null → loop ends.
21. Returns `Expr{0}`.

**C++ frames live simultaneously**: at most ~9 (trampoline + `CallEvaluated` +
`FnCall` + `EvaluateBody` + `Evaluate` + `EvalVisitor(List)` + `If::Call` +
`EvalVisitor(List for countdown call)` + one level of arg evaluation). Constant
regardless of `n`.

---

## 7. Tail-position propagation through `if` and `let`

### 7.1 `if`

`(if ...)` is a pure `Callable` (not a `Function` subclass) that manually
evaluates its three sub-expressions. It is tail-position-transparent: whichever
branch it selects is itself in tail position if `if` was in tail position.

The protocol:

```cpp
// simpl/built_in/control_flow.cc
Expr If::Call(Interpreter* interpreter, args_type&& exprs) {
  bool was_tail = interpreter->tail_position();
  interpreter->set_tail_position(false);        // condition is NOT tail
  auto cond = interpreter->Evaluate(*i++);
  // ...
  interpreter->set_tail_position(was_tail);     // restore for chosen branch
  if (IsTruthy(cond)) return interpreter->Evaluate(*then);
  else                return interpreter->Evaluate(*otherwise);
}
```

And in `EvalVisitor::operator()(List&&)`, before calling any non-`UserFn` callable,
`tail_position_` has already been cleared by `std::exchange`. The flag is
**selectively restored** only for `built_in::If`:

```cpp
// simpl/interpreter.cc — EvalVisitor::operator()(List&&)
if (in_tail && std::dynamic_pointer_cast<built_in::If>(callable)) {
  interpreter->set_tail_position(true);
}
auto call_result = callable->Call(interpreter, std::move(args));
```

Why only `If` and not every `Callable`? See §9.

### 7.2 `let`

`let` uses `EvaluateBody` for its body, which unconditionally marks the last body
expression as tail position — independently of whether `let` itself was in tail
position:

```cpp
// simpl/built_in/control_flow.cc
return interpreter->EvaluateBody(ExprList(i, exprs.end()), env);
```

`EvaluateBody`:

```cpp
// simpl/interpreter.cc
Expr Interpreter::EvaluateBody(const ExprList& exprs,
                               std::shared_ptr<Environment> env) {
  // ... env swap ...
  for (auto it = exprs.begin(); it != exprs.end();) {
    auto next = std::next(it);
    if (next == exprs.end()) tail_position_ = true;  // last expr only
    result = Evaluate(Expr{*it});
    tail_position_ = false;                          // always reset after
    it = next;
  }
  return result;
}
```

Because `EvaluateBody` sets `tail_position_=true` itself, `let` does **not** need
its `tail_position_` flag restored before entering — and accordingly, no special
case for `built_in::Let` appears in `EvalVisitor`.

User-defined functions use the same mechanism:

```cpp
// simpl/user_fn.cc
return interpreter->EvaluateBody(definition_.body(), env);
```

---

## 8. The callable class hierarchy and why only `UserFn` is trampolined

```
Callable  (pure virtual Call)
├── Function  (Call evaluates args, then dispatches to FnCall)
│   ├── UserFn          ← TCO applies here
│   ├── built_in::Sum   ← TCO does NOT apply
│   ├── built_in::Eval  ← TCO does NOT apply
│   ├── built_in::Head  ← TCO does NOT apply
│   ├── built_in::Do    ← TCO does NOT apply (see §9.3)
│   └── ... (all other strict built-ins)
├── built_in::If    ← tail-position-transparent (see §7.1)
├── built_in::Let   ← tail-position-transparent via EvaluateBody (see §7.2)
├── built_in::And   ← NOT TCO'd (see §9.2)
└── built_in::Or    ← NOT TCO'd (see §9.2)
```

The TCO detection in `EvalVisitor::operator()(List&&)` casts to `UserFn`
specifically, not the broader `Function`:

```cpp
auto fn = std::dynamic_pointer_cast<UserFn>(callable);
if (in_tail && fn && !fn->is_lazy()) {
  // evaluate args, return TailCall
}
```

Casting to `Function` would incorrectly TCO every strict built-in (producing
`TailCall` values that `*`, `+`, `eval`, `head`, etc. would receive as arguments
rather than computed results).

---

## 9. What does NOT get TCO and why

### 9.1 Strict built-ins (`*`, `+`, `eval`, `head`, …)

These are all `Function` subclasses with `lazy_=false`. `Function::Call` evaluates
every argument before dispatching to `FnCall`:

```cpp
// simpl/function.cc
Expr Function::Call(Interpreter* interpreter, args_type&& exprs) {
  if (lazy_) return FnCall(interpreter, std::move(exprs));
  std::transform(make_move_iterator(exprs.begin()), ...,
    [interpreter](Expr&& expr) { return interpreter->Evaluate(std::move(expr)); });
  return FnCall(interpreter, std::move(exprs));
}
```

A call like `(* n (f (- n 1)))` is **not** a tail call even when it appears as
the last expression in a body: `*` needs to wait for **both** `n` and
`(f (- n 1))` before it can compute anything. The call to `f` is an argument
evaluation, not a tail call. Treating `*` in tail position as a TCO candidate
would cause `Function::Call`'s argument evaluation loop to see `tail_position_=true`
and turn `(f (- n 1))` into a `TailCall` before `*` can consume it — breaking
multiplication.

The `EvalVisitor` therefore never sets `tail_position_=true` before calling a
non-`If` callable. Strict built-ins always run in a clear-flag environment.

### 9.2 `and` and `or`

`And` and `Or` are pure `Callable` subclasses (not `Function`) that evaluate their
arguments one at a time using `interpreter->Evaluate` inside a `std::accumulate`
loop:

```cpp
// simpl/built_in/logic.cc
Expr And::Call(Interpreter* interpreter, args_type&& exprs) {
  return std::accumulate(
      make_move_iterator(exprs.begin()), make_move_iterator(exprs.end()),
      Expr{true}, [interpreter](Expr&& acc, Expr&& expr) -> Expr {
        return IsTruthy(acc) ? interpreter->Evaluate(std::move(expr)) : acc;
      });
}
```

If `tail_position_=true` were set before `And::Call`, the **first** argument
evaluation would pick it up via `std::exchange` in `EvalVisitor::operator()(List&&)`,
potentially turning `(f ...)` in the first position into a `TailCall`. The
accumulate loop would then pass that `TailCall` to `IsTruthy`, which returns
`true` for any non-nil/non-false value — breaking short-circuit semantics
entirely.

### 9.3 `do`

`Do` is a `Function` subclass. `Function::Call` evaluates **all** arguments before
`FnCall` runs, leaving `Do::FnCall` with already-computed values:

```cpp
// simpl/built_in/control_flow.cc
Expr Do::FnCall(Interpreter*, args_type&& args) {
  return std::move(args.back());
}
```

By the time `FnCall` executes, the last argument — the one that might have been a
tail call — is already fully evaluated. Supporting TCO through `do` would require
restructuring `Do` as a pure `Callable` that evaluates its body sequentially via
`EvaluateBody`, similar to `Let`. This is left as a future improvement.

### 9.4 Lazy user functions (`lazy-fn`)

Lazy functions (`lazy_=true`) receive **unevaluated** argument expressions, which
they may inspect or selectively evaluate. The caller cannot eagerly evaluate the
arguments and package them into a `TailCall` — that would destroy the laziness
guarantee. The check `!fn->is_lazy()` excludes them:

```cpp
if (in_tail && fn && !fn->is_lazy()) { /* TailCall path */ }
```

Lazy functions can still make tail calls internally: `EvaluateBody` marks the last
body expression, so if the last expression in a lazy function's body is a call to
a non-lazy `UserFn`, that call will correctly produce a `TailCall`.

### 9.5 Non-self mutual recursion across `eval`

```lisp
(defn f [n] (eval (list 'g (- n 1))))
(defn g [n] (eval (list 'f (- n 1))))
```

The `eval` built-in is a `Function` subclass. When `eval` is in tail position, it
is called normally (no `TailCall` treatment). Inside `eval`, it calls
`interpreter->Evaluate` on its argument — a fresh call path that does not inherit
the outer tail context. Any tail call inside the `eval`'d expression starts a new
trampoline. Stack depth is bounded by the depth of nested `eval` calls, not the
recursion depth.

---

## 10. C++ call-stack depth analysis

For a simple tail-recursive function, the constant C++ stack depth per trampoline
iteration is approximately:

```
trampoline while-loop body (EvalVisitor::operator()(List&&))
  └─ Function::CallEvaluated
       └─ UserFn::FnCall
            └─ Interpreter::EvaluateBody
                 └─ Interpreter::Evaluate
                      └─ EvalVisitor::operator()(List&&)   [if expr]
                           └─ If::Call
                                └─ Interpreter::Evaluate
                                     └─ EvalVisitor::operator()(List&&)  [f call]
                                          ← returns TailCall here
```

That is roughly 9 C++ frames per logical recursion step, independent of `n`.
Without TCO, each step added one frame, so `countdown(1000000)` would require
roughly 1,000,000 frames.

The `TailCallOptimizationSelfRecursion` test in `simpl/interpreter_test.cc`
verifies this concretely:

```cpp
EXPECT_EQ(
    std::get<int_type>(Eval("(defn countdown [n] "
                            "  (if (= n 0) 0 (countdown (- n 1)))) "
                            "(countdown 1000000)")),
    int_type{0});
```

---

## 11. Modified files reference

| File | Change |
|---|---|
| `simpl/ast.hh` | `struct TailCall`; added to `ExprBase` variant; `operator<<` declaration |
| `simpl/ast.cc` | `operator<<` definition; `ExprPrinter` overload; `Hash` handler |
| `simpl/callable.hh` | (unchanged) |
| `simpl/function.hh` | `is_lazy()` accessor; `CallEvaluated()` method; `#include <utility>` |
| `simpl/function.cc` | (unchanged) |
| `simpl/interpreter.hh` | `tail_position_` field; `tail_position()`/`set_tail_position()` accessors; `EvaluateBody()` declaration |
| `simpl/interpreter.cc` | `EvalVisitor::operator()(TailCall&&)` passthrough; `EvalVisitor::operator()(List&&)` — `std::exchange`, `TailCall` emission, selective `If` propagation, trampoline loop; `EvaluateBody` implementation |
| `simpl/user_fn.cc` | `FnCall` calls `EvaluateBody` instead of `Evaluate` |
| `simpl/built_in/control_flow.cc` | `If::Call` saves/restores tail context; `Let::Call` uses `EvaluateBody` |
| `simpl/interpreter_test.cc` | `TailCallOptimizationSelfRecursion` test |
| `examples/fibonacci.simpl` | Replaced exponential naive fib with tail-recursive accumulator |

---

## 12. Extending TCO to new callables

### Adding a new user-visible control-flow form

If you add a new built-in that is a pure `Callable` (not a `Function` subclass)
and it is **tail-position-transparent** — meaning its last evaluated sub-expression
is in tail position when the form itself is in tail position — there are two routes:

**Route A: use `EvaluateBody`** (preferred, matches `let`)

Have the new form call `interpreter->EvaluateBody(body, env)` for the portion of
its sub-expressions that are in tail position. `EvaluateBody` automatically marks
the last expression, requiring no changes to `EvalVisitor`.

**Route B: read `tail_position_` directly** (matches `if`)

Have the new form read `interpreter->tail_position()` at entry, clear it for
non-tail evaluations, and restore it before evaluating the tail sub-expression.
Then add a `dynamic_pointer_cast` check in `EvalVisitor::operator()(List&&)`:

```cpp
// In interpreter.cc, EvalVisitor::operator()(List&&):
if (in_tail && (std::dynamic_pointer_cast<built_in::If>(callable) ||
                std::dynamic_pointer_cast<built_in::MyNewForm>(callable))) {
  interpreter->set_tail_position(true);
}
```

Route A is cleaner: it is self-contained within the new form and requires no
changes to `EvalVisitor`.

### Making `do` TCO-aware

Convert `Do` from a `Function` subclass to a pure `Callable`, evaluate all but the
last argument normally, and evaluate the last argument via `EvaluateBody`:

```cpp
// Hypothetical future Do::Call:
Expr Do::Call(Interpreter* interpreter, args_type&& exprs) {
  ExprList all(std::move_iterator(exprs.begin()),
               std::move_iterator(exprs.end()));
  return interpreter->EvaluateBody(all, nullptr);
}
```

No changes to `EvalVisitor` would be needed.

### Making `and`/`or` TCO-aware

The short-circuit nature of `and`/`or` means only the **last expression actually
evaluated at runtime** is in tail position, and that depends on the runtime
truthiness of preceding expressions. A correct implementation would need to run the
trampoline after the short-circuit decision:

```cpp
// Hypothetical TCO-aware Or::Call:
Expr Or::Call(Interpreter* interpreter, args_type&& exprs) {
  auto it = make_move_iterator(exprs.begin());
  auto end = make_move_iterator(exprs.end());
  while (it != end) {
    bool is_last = (std::next(it.base()) == exprs.end());
    if (is_last) interpreter->set_tail_position(true);
    auto val = interpreter->Evaluate(*it++);
    interpreter->set_tail_position(false);
    if (is_last || IsTruthy(val)) return val;
  }
  return Expr{false};
}
```

The trampoline in `EvalVisitor` would then resolve any `TailCall` returned when
`Or` is called from a non-tail context, as it already does for other callables.
