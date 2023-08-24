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

## What's working

### Primitive data types

Integer, floating-point number, string, boolean, and `nil`

### Arithmetic and logic operators

The usual built-in operators and functions: `+`, `-`, `*`, `/`, `%`, `not`, `and`, `or`.

Comparisons: `=`, `>`, `>=`, `<`, `<=`.

### Control flow

`if`, `do`, and recursion.

### Var and function Definitions

``` clojure
(def answer 42)

```

``` clojure
(defn fib [n]
  (if (or (= n 0) (= n 1))
    1
    (+ (fib (- n 1)) (fib (- n 2)))))
```

### Local binding (and closure)

``` clojure
(let [a 5]
  (def plus (fn [b] (+ a b))))

(plus 2)  ; => 7
```

### List

``` clojure
(cons 1 nil)  ; => (1)
(cons 1 '(2 3 4))  ; => (1 2 3 4)

(head '(1 2 3 4))  ; => 1
(tail '(1 2 3 4))  ; => (2 3 4)
```

### Vector

``` clojure
(def a [1 2 3 4])

(head a)   ; => 1
(tail a)   ; => (2 3 4)
(get a 2)  ; => 3
```

### Map

```clojure
(def person {:name "John Snow"
             :age 35
             :alive true
             :knows nil})

(assert (= (:name person) "John Snow"))
(assert (= (:age person) 35))
(assert (:alive person))
```

### `eval`

``` clojure
(eval '(+ 1 2 3))  ;; => 6
```

Note: `eval` in Simpl can access the lexical scope, which is different from most
other LISP languages. In other words, the following works:

``` clojure
(let [a 1]
  (eval '(+ a 2)))  ;; => 3
```

I might revisit this decision.

### Function
#### Lambda function as first-class value

``` clojure
(defn cons [h t]  ;; cons is a built-in function but can be overridden.
  (fn [f] (f h t)))

(defn car [l]
  (l (fn [h _] h)))

(defn cdr [l]
  (l (fn [_ t] t)))

(defn printl [l]
  (if l
    (do (print (car l))
        (printl (cdr l)))
    (println)))

(def lst (cons 1 (cons 2 (cons 3 nil))))

(printl lst)  ;; prints `1 2 3`
```


#### Lazy function

``` clojure
(let
    [f (lazy-fn [a]
        (eval (head a)))]
    (f ((/ 2 1) (/ 2 0))))  ;; => 2
```

#### Variadic function

``` clojure
(defn foo [a & b]
    (head b))

(foo 1 2 3)  ;; => 2
```

#### Arrows

``` clojure
(-> 1 (+ 2 3) (* 2))  ;; => 12
```

## Coming soon

- More control flow options
- `loop`, `recur` and tail-call optimization
- A richer standard library

## Longer-term road map

- Coroutines
- Threading
- Maybe a virtual machine for performance

## Contributing

- Bug reports and pull requests are welcome
- Before submitting a pull request:
  - Make sure new code is covered by tests
  - Make sure `scripts/check` passes with no error
    - Install the newest version of `cpplint` to support C++20:
      ```shell
      pip install git+https://github.com/cpplint/cpplint.git
      ```
