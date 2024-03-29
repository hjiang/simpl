// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_ASSERT_HH_
#define SIMPL_BUILT_IN_ASSERT_HH_

#include <stdexcept>
#include <string>

#include "simpl/function.hh"
#include "simpl/interpreter.hh"
#include "simpl/interpreter_util.hh"

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

#endif  // SIMPL_BUILT_IN_ASSERT_HH_
