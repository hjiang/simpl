// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/function.h"

#include <algorithm>
#include <iterator>
#include <utility>

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {

Expr Function::Call(Interpreter* interpreter, args_type&& exprs) {
  if (lazy_) {
    return FnCall(interpreter, std::move(exprs));
  }
  std::transform(make_move_iterator(exprs.begin()),
                 make_move_iterator(exprs.end()), exprs.begin(),
                 [interpreter](Expr&& expr) {
                   return interpreter->Evaluate(std::move(expr));
                 });
  return FnCall(interpreter, std::move(exprs));
}
}

}  // namespace simpl
