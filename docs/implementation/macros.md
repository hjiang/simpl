# Macro System in Simpl

## 1. Motivation

Before this feature, Simpl had no mechanism for users to define syntactic
abstractions. The `->` threading macro in `simpl_lib/arrows.simpl` worked around
this with a `lazy-fn` + `eval` pattern:

```lisp
(def -> (lazy-fn [forms]
  ...))
```

This approach is fragile: it cannot expand to multiple statements, it does not
compose with other macros, and it does not produce readable expansion output.

A proper macro system allows users to define transformations over unevaluated code
at call time, enabling patterns like:

```lisp
(defmacro when [cond & body]
  `(if ~cond (do ~@body) nil))

(when (> n 0)
  (println "positive")
  (process n))
```

The macro receives its arguments as unevaluated AST nodes, constructs a new
expression using quasiquote syntax, and the interpreter evaluates that expression
in the caller's environment — all transparently.

---

## 2. Overview of the four new constructs

| Syntax | Token | `Quoted::Kind` | Meaning |
|--------|-------|----------------|---------|
| `'x`   | `kQuote` (existing) | `kQuote` | Quote — suppress evaluation, return AST as data |
| `` `x `` | `kBacktick` | `kSyntaxQuote` | Syntax-quote — template; most forms kept as data, `~` and `~@` holes evaluated |
| `~x`   | `kTilde` | `kUnquote` | Unquote — inside a syntax-quote, evaluate `x` and substitute |
| `~@x`  | `kTildeSplice` | `kSplice` | Splice-unquote — inside a syntax-quote, evaluate `x` (must be a list) and splice its elements into the enclosing list |

All four are represented as `Quoted` values in the AST, differentiated by the
`kind_` field introduced in this implementation.

---

## 3. Stage 1 — Lexer: three new token types

Three new enum values are added to `Token::Type` after `kQuote`:

```cpp
// simpl/token.hh
kQuote,
kBacktick,
kTilde,
kTildeSplice,
```

The scanner handles them in `ScanToken()`:

```cpp
// simpl/lexer.cc
case '`':
  AddToken(Token::kBacktick);
  break;
case '~':
  AddToken(Match('@') ? Token::kTildeSplice : Token::kTilde);
  break;
```

The `~@` two-character token uses the existing `Match()` pattern — the same
mechanism `!=`, `<=`, `>=` use. `Match('@')` peeks at the next character,
consumes it if it matches, and returns whether it matched.

`CanEndNumber` is also extended to include `` ` `` and `~` so that a number
immediately followed by a quoting character (e.g., `3`x`) is treated as an error
rather than an incomplete number scan:

```cpp
bool CanEndNumber(char c) {
  return isspace(c) || c == ')' || c == ']' || c == '}' || c == ',' ||
         c == '`' || c == '~';
}
```

---

## 4. Stage 2 — AST: `Quoted::Kind`

### 4.1 Design choice: extend `Quoted` vs add new variant alternatives

The `ExprBase` variant already has 13 alternatives. Each new alternative requires:

- A new `Hash::operator()` lambda
- A new `ExprPrinter::operator()` overload
- A passthrough in `EvalVisitor::operator()(auto&&)`
- Updates to every downstream `std::visit` that does not use a catch-all

Rather than adding three new alternatives (`SyntaxQuoted`, `Unquoted`,
`SpliceUnquoted`), the implementation extends `Quoted` with a `Kind` discriminant:

```cpp
// simpl/ast.hh
class Quoted {
 public:
  enum class Kind { kQuote, kSyntaxQuote, kUnquote, kSplice };

  explicit Quoted(const Expr& expr, Kind kind = Kind::kQuote);
  Kind kind() const { return kind_; }
  // ...
 private:
  std::unique_ptr<Expr> expr_;
  Kind kind_;
};
```

The default `Kind::kQuote` preserves the existing `Quoted(expr)` constructor
syntax used throughout the parser, requiring no changes to callers that only
create plain quotes.

The parser maps each new token to its kind:

```cpp
// simpl/parser.cc
case Token::Type::kQuote:
  return Expr{Quoted(ParseExpr(), Quoted::Kind::kQuote)};
case Token::Type::kBacktick:
  return Expr{Quoted(ParseExpr(), Quoted::Kind::kSyntaxQuote)};
case Token::Type::kTilde:
  return Expr{Quoted(ParseExpr(), Quoted::Kind::kUnquote)};
case Token::Type::kTildeSplice:
  return Expr{Quoted(ParseExpr(), Quoted::Kind::kSplice)};
```

And `operator<<` distinguishes them in output:

```cpp
// simpl/ast.cc
std::ostream& operator<<(std::ostream& os, const Quoted& qt) {
  switch (qt.kind()) {
    case Quoted::Kind::kQuote:       return os << '\'' << qt.expr();
    case Quoted::Kind::kSyntaxQuote: return os << '`'  << qt.expr();
    case Quoted::Kind::kUnquote:     return os << '~'  << qt.expr();
    case Quoted::Kind::kSplice:      return os << "~@" << qt.expr();
  }
  return os;
}
```

### 4.2 Impact on hash and equality

`Quoted::hash()` XORs in the `kind_` value so that `'x`, `` `x ``, `~x`, and
`~@x` all produce distinct hashes even when wrapping the same expression:

```cpp
std::size_t hash() const {
  return hash(*expr_) ^ std::hash<uint32_t>{}(0xf00dcafe) ^
         std::hash<int>{}(static_cast<int>(kind_));
}
```

`Quoted::operator==` was updated to require matching `kind_`:

```cpp
bool Quoted::operator==(const Quoted& other) const {
  return kind_ == other.kind_ && *expr_ == *other.expr_;
}
```

---

## 5. Stage 3 — Interpreter: quasiquote evaluation

### 5.1 `ProcessSyntaxQuote`

`ProcessSyntaxQuote` is a free function (in an anonymous namespace in
`interpreter.cc`) that recursively walks a syntax-quote template and applies
substitution rules:

```cpp
Expr ProcessSyntaxQuote(Interpreter* interpreter, Expr&& expr) {
  return std::visit(
      Overload{
          [&](List&& list) -> Expr {
            List result;
            for (auto& elem : list) {
              if (auto* qt = std::get_if<Quoted>(&elem)) {
                if (qt->kind() == Quoted::Kind::kUnquote) {
                  result.push_back(interpreter->Evaluate(Expr{qt->expr()}));
                  continue;
                }
                if (qt->kind() == Quoted::Kind::kSplice) {
                  auto spliced = interpreter->Evaluate(Expr{qt->expr()});
                  auto& splice_list = std::get<List>(spliced);
                  for (auto& e : splice_list) result.push_back(std::move(e));
                  continue;
                }
              }
              result.push_back(ProcessSyntaxQuote(interpreter, std::move(elem)));
            }
            return Expr{std::move(result)};
          },
          [&](Quoted&& qt) -> Expr {
            if (qt.kind() == Quoted::Kind::kUnquote)
              return interpreter->Evaluate(Expr{qt.expr()});
            if (qt.kind() == Quoted::Kind::kSplice)
              throw std::runtime_error("~@ used outside of a list");
            return Expr{std::move(qt)};
          },
          [](auto&& other) -> Expr {
            return Expr{std::forward<decltype(other)>(other)};
          }},
      std::move(expr));
}
```

The three cases are:

**List:** Iterate elements. For each element:
- If it is a `kUnquote` Quoted → evaluate the inner expression, substitute.
- If it is a `kSplice` Quoted → evaluate, `std::get<List>` the result, splice all
  elements into the parent list.
- Otherwise → recurse into `ProcessSyntaxQuote` (handles nested lists and keeps
  symbols and literals as data).

**Top-level Quoted:**
- `kUnquote` → evaluate (unquote at the top level of a syntax-quote just evaluates).
- `kSplice` → error (`~@` at the top level has no list to splice into).
- Other kinds → keep as data.

**Everything else** (integers, floats, booleans, nil, symbols, keywords, maps,
vectors) → returned unchanged. Symbols in a syntax-quote are kept as symbol data,
not looked up in the environment — exactly like regular quote.

**Why `List` but not `Vector`?** Simpl macro templates are written as lists
(parenthesised forms), and `~@` is only meaningful when splicing into a list
position. Vector support could be added similarly if needed.

### 5.2 EvalVisitor handler for `Quoted`

The `EvalVisitor::operator()(Quoted&&)` previously had a single case (return the
inner expression). It is replaced with a four-way dispatch:

```cpp
Expr operator()(Quoted&& expr) {
  switch (expr.kind()) {
    case Quoted::Kind::kQuote:
      return expr.expr();                                   // existing behavior
    case Quoted::Kind::kSyntaxQuote:
      return ProcessSyntaxQuote(interpreter, Expr{expr.expr()});
    case Quoted::Kind::kUnquote:
      return interpreter->Evaluate(Expr{expr.expr()});      // evaluate the inner expr
    case Quoted::Kind::kSplice:
      throw std::runtime_error("~@ used outside of syntax-quote");
  }
  return Expr{nullptr};
}
```

The `kUnquote` case at the top level evaluates the inner expression. This means
`~x` outside a syntax-quote is equivalent to `(eval x)` — not particularly useful,
but consistent. `kSplice` at the top level is always an error.

---

## 6. Stage 4 — `UserMacro`

`UserMacro` is a minimal subclass of `UserFn` in `simpl/user_macro.hh`:

```cpp
class UserMacro : public UserFn {
 public:
  explicit UserMacro(FnDef&& definition,
                     std::shared_ptr<Interpreter::Environment> closure = nullptr)
      : UserFn(std::move(definition), closure, /*lazy=*/true) {}
};
```

Its only behavioural difference from `UserFn` is that it **always passes
`lazy=true`** to the `UserFn` constructor. This means `Function::Call` will not
evaluate the arguments before dispatching to `FnCall` — the macro body receives
the raw unevaluated AST nodes exactly as they appeared at the call site.

The `UserMacro` type has no additional methods. Its sole purpose is to be
identifiable by type via `std::dynamic_pointer_cast<UserMacro>` in the interpreter,
so that the macro expansion path can be taken instead of the normal function call
path.

**Class hierarchy after this change:**

```
Callable  (pure virtual Call)
├── Function  (Call evaluates args unless lazy_, then dispatches to FnCall)
│   ├── UserFn          ← evaluates or skips args based on lazy_
│   │   └── UserMacro   ← always lazy; identified by type for expansion
│   ├── built_in::Macroexpand
│   └── ... (all other Function subclasses)
├── built_in::Defmacro  ← pure Callable (receives unevaluated args)
└── ... (other pure Callables: If, And, Or, Def, Defn, ...)
```

---

## 7. Stage 5 — `defmacro` and `macroexpand`

### 7.1 `defmacro`

`Defmacro` is a pure `Callable` (not a `Function`) so that it receives its
arguments unevaluated — name, parameter vector, and body are all AST nodes.

```cpp
// simpl/built_in/macro.cc
Expr Defmacro::Call(Interpreter* interpreter, args_type&& exprs) {
  auto name = std::get<Symbol>(exprs.front()).name;

  // Parse params the same way as Fn::Call
  auto params = std::get<Vector>(*std::next(exprs.begin()));
  // ... split positional params from & rest-param ...

  FnDef::body_t body;
  // ... collect body expressions ...

  auto macro = std::make_shared<UserMacro>(
      FnDef(std::move(param_names), std::move(body), std::move(param_rest)),
      interpreter->env());
  interpreter->env()->Define(name, Expr{callable_ptr_t{macro}});
  return Expr{nullptr};
}
```

`defmacro` reuses the same parameter-parsing logic as `fn` (splitting at `&` for
rest parameters). The resulting `UserMacro` is stored in the global environment
under the given name, wrapped in a `callable_ptr_t`. The macro closes over the
environment at the time of definition, just like `defn`.

Variadic macros work the same way as variadic functions:

```lisp
(defmacro when [cond & body]
  `(if ~cond (do ~@body) nil))
```

Here `cond` receives the first unevaluated argument and `body` receives the rest
as a `List`.

### 7.2 `macroexpand`

`Macroexpand` is a `Function` subclass (eager), so the form passed to it is
evaluated before `FnCall` runs. The typical call is `(macroexpand '(when true 1 2))`:
the quote evaluates to the list `(when true 1 2)`.

```cpp
Expr Macroexpand::FnCall(Interpreter* interpreter, args_type&& args) {
  auto form = std::get<List>(std::move(args.front()));
  auto first = std::get<Symbol>(form.front()).name;
  auto resolved = interpreter->env()->Get(first);
  auto macro = std::dynamic_pointer_cast<UserMacro>(
                   std::get<callable_ptr_t>(resolved));
  if (!macro) throw std::runtime_error("`macroexpand`: first element is not a macro.");

  auto it = form.begin(); ++it;
  ExprList macro_args;
  std::move(it, form.end(), std::back_inserter(macro_args));
  return macro->Call(interpreter, std::move(macro_args));
}
```

`macroexpand` calls the macro with the unevaluated argument forms and returns the
expanded result **without re-evaluating it**. This is useful for debugging macro
templates:

```lisp
(macroexpand '(when true 1 2))
; => (if true (do 1 2) nil)
```

---

## 8. Macro expansion in list evaluation

The key site where macros differ from normal function calls is
`EvalVisitor::operator()(List&&)`. After resolving the first element to a
callable, a `UserMacro` check is inserted **before** the normal TCO and
function-dispatch logic:

```cpp
// simpl/interpreter.cc — EvalVisitor::operator()(List&&)
auto macro = std::dynamic_pointer_cast<UserMacro>(callable);
if (macro) {
  auto it = list.begin(); ++it;
  ExprList args;
  std::move(it, list.end(), std::back_inserter(args));
  auto expanded = macro->Call(interpreter, std::move(args));
  return interpreter->Evaluate(std::move(expanded));
}
```

The sequence:

1. Resolve the first element of the list to a callable (same as always).
2. If it is a `UserMacro`: collect the remaining list elements as **unevaluated** args.
3. Call the macro — it returns an expanded `Expr` (the result of evaluating its body,
   which is typically a syntax-quoted template).
4. Re-evaluate the expanded expression in the **current** (caller's) environment.

Step 4 is the critical difference between macros and lazy functions: a lazy
function's return value is the final result, whereas a macro's return value is
code to be run again.

**Why before the TCO check?** The `UserMacro` check must precede the `UserFn` TCO
check because `UserMacro` IS a `UserFn` (via inheritance), so the `UserFn` cast
would also succeed. If the TCO path ran first, the interpreter would attempt to
evaluate the macro's arguments eagerly and return a `TailCall` sentinel, bypassing
macro expansion entirely.

The check `if (in_tail && fn && !fn->is_lazy())` that precedes the TCO path would
actually skip `UserMacro` because `is_lazy()` returns `true` for macros — but the
explicit `UserMacro` check with immediate expansion and re-evaluation is clearer
and handles the `in_tail` flag correctly without special-casing.

---

## 9. End-to-end trace: `when`

```lisp
(defmacro when [cond & body]
  `(if ~cond (do ~@body) nil))

(when (> x 0) (println x) x)
```

**Defining the macro:**

1. `EvalVisitor(List{defmacro, when, [cond & body], `(if ~cond (do ~@body) nil)})`.
2. `defmacro` is a `Callable` — args are NOT evaluated before `Call`.
3. `Defmacro::Call` parses `name="when"`, `params=[cond & body]` (positional: `cond`,
   rest: `body`), body: `` [`(if ~cond (do ~@body) nil)] ``.
4. Creates `UserMacro{FnDef{["cond"], ["(if ...)"], "body"}, current_env}`.
5. `env->Define("when", callable_ptr_t{macro})`.

**Calling the macro:**

1. `EvalVisitor(List{when, (> x 0), (println x), x})`.
2. Evaluate `when` → `callable_ptr_t` to `UserMacro`.
3. `dynamic_pointer_cast<UserMacro>` succeeds.
4. Collect unevaluated args: `{(> x 0), (println x), x}`.
5. Call `macro->Call(interpreter, {(> x 0), (println x), x})`.
   - `Function::Call` sees `lazy_=true` → skips evaluation, calls `FnCall` directly.
   - `UserFn::FnCall`:
     - Binds `cond = (> x 0)` (unevaluated List).
     - Binds `body = ((println x) x)` (List of two unevaluated exprs).
   - Evaluates body: `` `(if ~cond (do ~@body) nil) ``.
   - `EvalVisitor(Quoted{kSyntaxQuote, List{if, ~cond, (do ~@body), nil}})`.
   - `ProcessSyntaxQuote(List{Symbol{if}, Quoted{cond,kUnquote}, List{do, Quoted{body,kSplice}}, nullptr})`:
     - `Symbol{if}` → kept as `Symbol{if}`.
     - `Quoted{cond, kUnquote}` → `interpreter->Evaluate(Symbol{cond})` → `List{>, x, 0}`.
     - `List{do, Quoted{body, kSplice}}` → recurse:
       - `Symbol{do}` → kept.
       - `Quoted{body, kSplice}` → evaluate `body` → `List{(println x), x}` → splice.
       - Result: `List{Symbol{do}, List{println, x}, Symbol{x}}`.
     - `nullptr` → kept.
   - Returns: `List{Symbol{if}, List{>,x,0}, List{do,(println x),x}, nullptr}`.
6. Back in `EvalVisitor`: `expanded = (if (> x 0) (do (println x) x) nil)`.
7. `interpreter->Evaluate(expanded)` — evaluates the expanded form normally.
   - `(> x 0)` is evaluated; if truthy, `(do (println x) x)` runs; else `nil`.

---

## 10. Extended equality for `=`

`=` only supported numeric and string equality, throwing `"Incomparable types."`
for nil, bool, Symbol, Keyword, and other Expr alternatives. Macro integration
tests needed `(= nil nil)`, `(= 'if 'if)`, and `(= true true)`.

The equality built-in (`Equals::FnCall`) was rewritten to use `Expr`'s inherited
`std::variant::operator==` for the same-type case, plus an explicit cross-numeric
path:

```cpp
bool ExprEqual(Expr&& lhs, Expr&& rhs) {
  if (lhs == rhs) return true;   // variant::== handles same-type equality
  // Cross-numeric: int vs float
  if (holds<int_type>(lhs) && holds<float_type>(rhs))
    return static_cast<float_type>(std::get<int_type>(lhs)) == std::get<float_type>(rhs);
  if (holds<float_type>(lhs) && holds<int_type>(rhs))
    return std::get<float_type>(lhs) == static_cast<float_type>(std::get<int_type>(rhs));
  return false;
}
```

`std::variant::operator==` first checks that both variants hold the same
alternative (by comparing `index()`), then compares the held values using their
own `operator==`. This works correctly for all alternatives in `ExprBase`:
`nullptr_t`, `bool`, `Symbol`, `Keyword`, `std::string`, `List`, `Vector`, `Map`,
`Quoted`, `callable_ptr_t`, and `TailCall`.

**Why not a constrained template visitor?** An earlier attempt used a constrained
template `operator()` in a visitor struct. The template deduction rules mean
`auto&&` (unconstrained) is an exact match for rvalue arguments, while
`const T&` requires reference binding — the unconstrained template won over the
more-specific one in practice. The `variant::operator==` approach avoids this
pitfall entirely.

The ordering operators (`>`, `<`, `>=`, `<=`) are unchanged and still throw
`"Incomparable types."` for non-numeric, non-string types.

---

## 11. Known limitations

**No hygienic macros.** A macro that introduces a binding name can shadow a
name in the caller's scope:

```lisp
(defmacro swap! [a b]
  `(let [tmp ~a] (def ~a ~b) (def ~b tmp)))
```

If the caller has a variable named `tmp`, this macro will shadow it. The standard
Clojure solution is `gensym` (generating unique symbol names) or fully hygienic
macro expansion. Neither is implemented.

**No nested syntax-quote depth tracking.** Clojure tracks the nesting depth of
syntax-quotes so that `` `(a `(b ~c)) `` only evaluates `c` at the inner level.
Simpl's `ProcessSyntaxQuote` does not track depth; nested syntax-quotes inside a
template will be processed immediately.

**Macros are not first-class values in the same way as functions.** A `UserMacro`
can be stored in a variable and passed around as a `callable_ptr_t`, but calling it
through an indirect reference will not trigger macro expansion — the `UserMacro`
check in `EvalVisitor` only fires when the macro is resolved by name at the head of
a list.

**`macroexpand` only expands one level.** `(macroexpand '(unless false 42))`
returns `(when (not false) 42)`, not the fully expanded `(if (not false) (do 42) nil)`.
A `macroexpand-all` function could recursively expand until stable.

---
