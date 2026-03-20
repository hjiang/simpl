# Simpl

[![Tests](https://github.com/hjiang/simpl/actions/workflows/run-tests.yml/badge.svg?branch=master)](https://github.com/hjiang/simpl/actions/workflows/run-tests.yml)
[![Lint](https://github.com/hjiang/simpl/actions/workflows/lint.yml/badge.svg?branch=master)](https://github.com/hjiang/simpl/actions/workflows/lint.yml)
[![codecov](https://codecov.io/gh/hjiang/simpl/branch/master/graph/badge.svg?token=9JRBMMPRZV)](https://codecov.io/gh/hjiang/simpl)

## Introduction

Simpl (pronounced "simple", abbreviation for **sim**ple **p**rogramming
**l**anguage or **simp**le **L**ISP depending on your preference) is a
programming language. It aims to be a LISP dialect suitable for scripting and
borrows many syntactic features from [Clojure](https://clojure.org).

It was motivated by the author's desire for a scripting language with nice
syntax and without large dependencies like the JVM. It is implemented in modern
C++ without any third-party dependency other than the standard library.

## How to use

Simpl isn't quite ready for day-to-day use yet, but you can play with it by
installing [Bazel](https://bazel.build) and running:

``` sh
bazel build -c opt //simpl:simpl
```

The executable will be at `bazel-bin/simpl/simpl`. It's both a REPL and a source
file interpreter.

The author uses a recent version of Clang and has not tested other compilers. Development environment can be set up using `nix` with the `flake.nix` file at the project root.

## Tutorial

The following is an introduction to implemented features.

### Comments

```clojure
; This is a comment. Everything after ; on a line is ignored.
```

### Primitive types

Simpl has integers, floating-point numbers, booleans, strings, and `nil`.

```clojure
42          ; integer
3.14        ; float
true        ; boolean
false
"hello"     ; string
nil         ; the absence of a value
```

### Arithmetic

All operations use prefix notation inside parentheses. Most operators accept
more than two arguments.

```clojure
(+ 1 2)        ; => 3
(- 10 3)       ; => 7
(* 2 3 4)      ; => 24
(/ 10 2)       ; => 5
(% 11 3)       ; => 2

; Integers and floats mix freely
(+ 1 2.5)      ; => 3.5
```

Negation is unary `-`, and unary `+` returns its argument unchanged:

```clojure
(- 7)          ; => -7
(+ 42)         ; => 42
```

### Comparisons and logic

```clojure
(= 1 1)        ; => true
(> 3 2)        ; => true
(< 3 2)        ; => false
(>= 3 3)       ; => true
(<= 2 3)       ; => true

; = also works for strings
(= "foo" "foo")  ; => true

; Integers and floats compare equal when their values match
(= 1 1.0)      ; => true
```

Boolean logic:

```clojure
(and true false)   ; => false
(or  true false)   ; => true
(not nil)          ; => true   — nil is falsy
(not false)        ; => true
(not 0)            ; => false  — only nil and false are falsy
```

### Variables

`def` binds a name in the current scope:

```clojure
(def answer 42)
(+ answer 1)       ; => 43
```

### Local bindings with `let`

`let` creates names that are only visible inside its body. Multiple bindings
can be listed in a single `let`:

```clojure
(let [x 3]
  (* x x))          ; => 9

(let [a 1
      b 2]
  (+ a b))          ; => 3
```

`let` returns the value of its last body expression.

### Control flow

`if` takes a condition, a then-expression, and an optional else-expression:

```clojure
(if true  "yes" "no")   ; => "yes"
(if false "yes" "no")   ; => "no"
(if nil   "yes")        ; => nil  — else defaults to nil
```

`do` evaluates a sequence of expressions and returns the last one. It is
useful when you need multiple steps in a branch:

```clojure
(do
  (println "step 1")
  (println "step 2")
  42)                   ; => 42
```

### Functions

`fn` creates an anonymous function:

```clojure
(fn [x] (* x x))

; Call it immediately
((fn [x] (* x x)) 5)   ; => 25

; Or bind it to a name
(def square (fn [x] (* x x)))
(square 7)              ; => 49
```

`defn` is a convenient shorthand for `def` + `fn`:

```clojure
(defn square [x]
  (* x x))

(square 9)              ; => 81
```

Functions are first-class values — they can be passed to other functions and
returned from them.

#### Closures

Functions capture the environment where they were created:

```clojure
(let [offset 10]
  (def add-offset (fn [n] (+ n offset))))

(add-offset 5)          ; => 15
```

#### Recursion and tail-call optimisation

Self-recursive functions are supported. Simpl performs tail-call optimisation
(TCO), so a tail-recursive function does not grow the call stack:

```clojure
(defn factorial [n]
  (if (= n 0)
    1
    (* n (factorial (- n 1)))))

(factorial 10)          ; => 3628800

; TCO in action — this counts down a million steps without stack overflow
(defn countdown [n]
  (if (= n 0) 0 (countdown (- n 1))))

(countdown 1000000)     ; => 0
```

#### Variadic functions

Use `& rest` to collect extra arguments into a list:

```clojure
(defn greet [greeting & names]
  (println greeting (head names)))

(greet "Hello" "Alice" "Bob")   ; prints: Hello Alice
```

### Collections

#### Lists

A quoted list is a sequence of values:

```clojure
'(1 2 3)               ; a list literal

(cons 0 '(1 2 3))      ; => (0 1 2 3)  — prepend an element
(head '(1 2 3))        ; => 1
(tail '(1 2 3))        ; => (2 3)
(empty? '())           ; => true
(empty? '(1))          ; => false
```

An empty list `()` is the same as `nil`.

#### Vectors

Vectors are written with square brackets and support random access:

```clojure
(def v [10 20 30])

(head v)               ; => 10
(tail v)               ; => (20 30)
(get  v 2)             ; => 30
(empty? [])            ; => true
```

#### Maps

Maps can use any value as a key — integers, strings, booleans, keywords, lists,
and more. Keywords are the most common choice and can be called as functions to
look up their value in a map:

```clojure
(def person {:name "Alice"
             :age  30
             :active true})

(:name person)         ; => "Alice"
(:age  person)         ; => 30
(get person :active)   ; => true

; Other key types work too
(def m {1 "one"  "two" 2  true "yes"})
(get m 1)              ; => "one"
(get m "two")          ; => 2
(get m true)           ; => "yes"
```

Maps can be nested:

```clojure
(def org {:teams [{:name "backend"} {:name "frontend"}]})

(-> org (:teams) (get 0) (:name))  ; => "backend"
```

### I/O

`print` writes values separated by spaces without a trailing newline.
`println` adds a newline at the end:

```clojure
(print   1 2.5 "hi" true nil)   ; prints: 1 2.5 hi true nil
(println "done")                 ; prints: done\n
```

### Quoting and `eval`

A single quote `'` prevents evaluation, returning the form as data:

```clojure
'(+ 1 2)               ; => (+ 1 2)  — a list, not a sum
'hello                 ; => hello    — a symbol, not a variable lookup
```

`eval` evaluates a data structure as code. In Simpl, `eval` has access to
the lexical scope where it is called:

```clojure
(eval '(+ 1 2))        ; => 3

(let [x 7]
  (eval '(* x 6)))     ; => 42
```

### Macros

Macros let you extend the language by transforming code at read time. A macro
receives its arguments *unevaluated* and returns a form that is then
evaluated in the caller's scope.

`defmacro` defines a macro. Inside the body, use syntax-quote `` ` ``,
unquote `~`, and splice-unquote `~@`:

```clojure
; Define a 'when' macro — like 'if' but without an else branch
(defmacro when [condition & body]
  `(if ~condition (do ~@body) nil))

(when true
  (println "it's true")
  42)                  ; prints "it's true", returns 42

(when false 99)        ; => nil
```

`macroexpand` shows the code a macro call produces without running it:

```clojure
(macroexpand '(when true 1 2))
; => (if true (do 1 2) nil)
```

Macros compose — a macro can call another macro:

```clojure
(defmacro unless [condition & body]
  `(when (not ~condition) ~@body))

(unless false "runs")  ; => "runs"
(unless true  "runs")  ; => nil
```

### Thread-first macro (`->`)

`->` passes a value through a chain of function calls, inserting it as the
*first* argument of each step. It is part of the standard library:

```clojure
(-> 1 (+ 2) (* 3))     ; => 9
; equivalent to (* (+ 1 2) 3)

; Useful for chaining collection lookups
(-> {:scores [10 20 30]}
    (:scores)
    (get 2))            ; => 30
```

### Lazy functions

`lazy-fn` creates a function whose arguments are *not* evaluated before the
call. This is useful for implementing short-circuit or conditional behaviour:

```clojure
(let [safe-div (lazy-fn [args]
                 (if (= (eval (head (tail args))) 0)
                   nil
                   (eval (head args))))]
  (safe-div (/ 10 2) (/ 10 0)))  ; => 5, no division-by-zero error
```

## Contributing

- Bug reports and pull requests are welcome
- Before submitting a pull request:
  - Make sure new code is covered by tests
  - Make sure `scripts/check` passes with no error
    - Install the newest version of `cpplint` to support C++20:
      ```shell
      pip install git+https://github.com/cpplint/cpplint.git
      ```
