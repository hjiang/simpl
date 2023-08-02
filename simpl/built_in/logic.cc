// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/logic.h"

namespace simpl {
namespace built_in {

Expr Or::Call(Interpreter* interpreter, const ExprList& exprs) {
  if (exprs.empty()) {
    throw std::runtime_error("`or` expects at least one argument");
  }
  for (const auto& expr : exprs) {
    decltype(auto) r = interpreter->Evaluate(expr);
    if (IsTruthy(r)) {
      return r;
    }
  }
  return interpreter->last_value();
}

Expr And::Call(Interpreter* interpreter, const ExprList& exprs) {
  if (exprs.empty()) {
    throw std::runtime_error("`and` expects at least one argument");
  }
  for (const auto& expr : exprs) {
    decltype(auto) r = interpreter->Evaluate(expr);
    if (!IsTruthy(r)) {
      return r;
    }
  }
  return interpreter->last_value();
}

}  // namespace built_in
}  // namespace simpl
