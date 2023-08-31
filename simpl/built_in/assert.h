// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_ASSERT_H_
#define SIMPL_BUILT_IN_ASSERT_H_

#include <stdexcept>
#include <string>

#include "simpl/function.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

class Assert : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override {
    if (!IsTruthy(args.front())) {
      auto reason = args.size() == 2 ? std::get<std::string>(args.back())
                                     : "Assertion failed.";
      throw std::runtime_error(reason);
    }
    return Expr{nullptr};
  }
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_ASSERT_H_
