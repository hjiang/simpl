// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/user_fn.hh"

#include <memory>

#include "simpl/ast.hh"
#include "simpl/interpreter.hh"

namespace simpl {

Expr UserFn::FnCall(Interpreter* interpreter, Function::args_type&& args) {
  auto env = make_shared<Interpreter::Environment>(closure_);
  auto arg = args.begin();
  for (const auto& param : definition_.params()) {
    env->Bind(param, *arg++);
  }
  if (!definition_.param_rest().empty()) {
    env->Bind(definition_.param_rest(), List(arg, args.end()));
  }
  // Make a copy of the body so that it doesn't get moved by Evaluate.
  return interpreter->Evaluate(ExprList{definition_.body()}, env);
}

}  // namespace simpl
