// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_ASSERT_H_
#define SIMPL_BUILT_IN_ASSERT_H_

#include <stdexcept>
#include <string>

#include "simpl/callable.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

class Assert : public Function {
 private:
  Interpreter::atom_value_type FnCall(Interpreter*,
                                      const args_type& args) override {
    if (!IsTruthy(args.front())) {
      throw std::runtime_error(std::get<std::string>(args.back()));
    }
    return nullptr;
  }
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_ASSERT_H_
