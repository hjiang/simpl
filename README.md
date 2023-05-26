# SimpL 

![Tests](https://github.com/hjiang/simpl/actions/workflows/run-tests.yml/badge.svg?branch=master)
![Lint](https://github.com/hjiang/simpl/actions/workflows/lint.yml/badge.svg?branch=master)
[![codecov](https://codecov.io/gh/hjiang/simpl/branch/master/graph/badge.svg?token=9JRBMMPRZV)](https://codecov.io/gh/hjiang/simpl)

## Introduction

SimpL (pronounced "simple") is a programming language. It aims to be a LISP
dialect suitable for scripting and borrows many syntactic features from
[Clojure](https://clojure.org).

It was motivated by the author's desire for a scripting language with nice
syntax and without large dependencies like the JVM. It is implemented in modern
C++ without any third-party dependency other than the standard library.

## How to use

SimpL isn't quite ready for day-to-day use yet, but you can play with it by
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

### Control flow

`if`, `do`, and recursion

### Var and function Definitions

``` clojure
(def answer 42)

```

``` clojure
(defn fab [n]
  (if (or (= n 0) (= n 1))
    1
    (+ (fab (- n 1)) (fab (- n 2)))))
```

### Local binding (and closure)

``` clojure
(let [a 5]
  (def plus (fn [b] (+ a b))))

(plus 2)  ; => 7
```

### Lambda function as first-class value

``` clojure
(defn cons [h t]
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

## What's coming soon

- More control flow options
- `loop`, `recur` and tail-call optimization
- Macros
- A standard library (include the battery)
- Standard data structures: `list`, `vector`, `map`, `set`

## Longer-term road map

- Coroutines
- Threading
- A virtual machine (maybe)

## Contributing

- Bug reports and pull requests are welcome
- More guidelines coming soon
