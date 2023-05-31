// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_LOGIC_H_
#define SIMPL_BUILT_IN_LOGIC_H_

#include "simpl/callable.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

class Not : public Function {
 public:
  int arity() const override { return 1; }

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter*,
                                          const args_type& args) override {
    return Interpreter::atom_value_type(
        static_cast<bool>(!IsTruthy(args.front())));
  }
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_LOGIC_H_
