// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/logic.h"

#include <numeric>

namespace simpl {
namespace built_in {

Expr Or::Call(Interpreter* interpreter, const ExprList& exprs) {
  return std::accumulate(
      exprs.begin(), exprs.end(), Expr{false},
      [interpreter](const Expr& acc, const Expr& expr) -> Expr {
        return IsTruthy(acc) ? acc : interpreter->Evaluate(expr);
      });
}

Expr And::Call(Interpreter* interpreter, const ExprList& exprs) {
  return std::accumulate(
      exprs.begin(), exprs.end(), Expr{true},
      [interpreter](const Expr& acc, const Expr& expr) -> Expr {
        return IsTruthy(acc) ? interpreter->Evaluate(expr) : acc;
      });
}

}  // namespace built_in
}  // namespace simpl
