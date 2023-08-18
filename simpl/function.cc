// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/function.h"

#include <algorithm>

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {

Expr Function::Call(Interpreter* interpreter, const ExprList& exprs) {
  if (lazy_) {
    return FnCall(interpreter, exprs);
  }
  // TODO(hjiang): pass the args by pointer and avoid unnecessary copy?
  args_type args;
  std::transform(
      exprs.cbegin(), exprs.cend(), std::back_inserter(args),
      [interpreter](const Expr& expr) { return interpreter->Evaluate(expr); });
  return FnCall(interpreter, args);
}

}  // namespace simpl
