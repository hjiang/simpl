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

Expr Function::Call(Interpreter* interpreter, args_iterator begin,
                    args_iterator end) {
  if (lazy_) {
    return FnCall(interpreter, begin, end);
  }
  std::transform(make_move_iterator(begin), make_move_iterator(end), begin,
                 [interpreter](Expr&& expr) {
                   return interpreter->Evaluate(std::move(expr));
                 });
  return FnCall(interpreter, begin, end);
}

// TODO(hjiang): remove this
Expr Function::FnCall(Interpreter*, args_iterator begin, args_iterator end) {
  return FnCall(nullptr, args_type(begin, end));
}

}  // namespace simpl
