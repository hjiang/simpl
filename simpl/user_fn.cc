// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/user_fn.h"

#include <memory>

#include "simpl/interpreter.h"

namespace simpl {

Expr UserFn::FnCall(Interpreter* interpreter, const Function::args_type& args) {
  auto env = make_shared<Interpreter::Environment>(closure_);
  auto arg = args.begin();
  for (const auto& param : definition_.params()) {
    env->Bind(param, *arg++);
  }
  return interpreter->Evaluate(definition_.body(), env);
}

}  // namespace simpl
