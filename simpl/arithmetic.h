// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_ARITHMETIC_H_
#define SIMPL_ARITHMETIC_H_

#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

namespace builtin_fn {

class Sum: public Callable {
 public:
  int arity() const override { return -1; }
 private:
  Interpreter::atom_value_type CallImpl(Interpreter* interpreter,
                                        const args_type& args) override;
};

class Substract: public Callable {
 public:
  int arity() const override { return -1; }
 private:
  Interpreter::atom_value_type CallImpl(Interpreter* interpreter,
                                        const args_type& args) override;
};

class Multiply: public Callable {
 public:
  int arity() const override { return -1; }
 private:
  Interpreter::atom_value_type CallImpl(Interpreter* interpreter,
                                        const args_type& args) override;
};

class Divide: public Callable {
 public:
  int arity() const override { return -1; }
 private:
  Interpreter::atom_value_type CallImpl(Interpreter* interpreter,
                                        const args_type& args) override;
};

class Modulo: public Callable {
 public:
  int arity() const override { return -1; }
 private:
  Interpreter::atom_value_type CallImpl(Interpreter* interpreter,
                                        const args_type& args) override;
};

}  // namespace builtin_fn

}  // namespace simpl

#endif  // SIMPL_ARITHMETIC_H_
