// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_COMPARISON_H_
#define SIMPL_COMPARISON_H_

#include <vector>

#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

namespace built_in {

class Equals : public Callable {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override;
};

class GreaterThan : public Callable {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override;
};

class GreaterThanOrEqualTo : public Callable {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override;
};

class LessThan : public Callable {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override;
};

class LessThanOrEqualTo : public Callable {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_COMPARISON_H_
