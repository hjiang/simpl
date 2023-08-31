// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/function.h"

#include <algorithm>
#include <iterator>
#include <utility>

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {

Expr Function::Call(Interpreter* interpreter, ExprList&& exprs) {
  if (lazy_) {
    return FnCall(interpreter, std::move(exprs));
  }
  // TODO(hjiang): pass the args by pointer and avoid unnecessary copy?
  args_type args;
  std::transform(make_move_iterator(exprs.begin()),
                 make_move_iterator(exprs.end()), std::back_inserter(args),
                 [interpreter](Expr&& expr) {
                   return interpreter->Evaluate(std::move(expr));
                 });
  return FnCall(interpreter, std::move(args));
}

}  // namespace simpl
