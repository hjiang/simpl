// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_ARITHMETIC_H_
#define SIMPL_BUILT_IN_ARITHMETIC_H_

#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

namespace built_in {

class Sum : public Function {
 public:
  int arity() const override { return -1; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter* interpreter,
                                          const args_type& args) override;
};

class Substract : public Function {
 public:
  int arity() const override { return -1; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter* interpreter,
                                          const args_type& args) override;
};

class Multiply : public Function {
 public:
  int arity() const override { return -1; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter* interpreter,
                                          const args_type& args) override;
};

class Divide : public Function {
 public:
  int arity() const override { return -1; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter* interpreter,
                                          const args_type& args) override;
};

class Modulo : public Function {
 public:
  int arity() const override { return -1; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter* interpreter,
                                          const args_type& args) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_ARITHMETIC_H_
