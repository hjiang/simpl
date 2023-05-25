// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_LOGIC_H_
#define SIMPL_LOGIC_H_

#include "simpl/callable.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace builtin_fn {

class Not : public Callable {
 public:
  int arity() const override { return 1; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override {
    return Interpreter::atom_value_type(
        static_cast<bool>(!IsTruthy(args.front())));
  }
};

}  // namespace builtin_fn

}  // namespace simpl

#endif  // SIMPL_LOGIC_H_
