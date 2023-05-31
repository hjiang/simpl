// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_COMPARISON_H_
#define SIMPL_BUILT_IN_COMPARISON_H_

#include <vector>

#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

namespace built_in {

class Equals : public Function {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter*,
                                          const args_type& args) override;
};

class GreaterThan : public Function {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter*,
                                          const args_type& args) override;
};

class GreaterThanOrEqualTo : public Function {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter*,
                                          const args_type& args) override;
};

class LessThan : public Function {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter*,
                                          const args_type& args) override;
};

class LessThanOrEqualTo : public Function {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter*,
                                          const args_type& args) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_COMPARISON_H_
