// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/function.h"

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {

Expr Function::Call(Interpreter* interpreter, const ExprList& exprs) {
  args_type args;
  for (auto& expr : exprs) {
    args.push_back(interpreter->Evaluate(expr));
  }
  return FnCall(interpreter, args);
}

}  // namespace simpl
