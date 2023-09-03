// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/logic.h"

#include <iterator>
#include <numeric>
#include <utility>

namespace simpl {
namespace built_in {

Expr Or::Call(Interpreter* interpreter, args_type&& exprs) {
  return std::accumulate(
      make_move_iterator(exprs.begin()), make_move_iterator(exprs.end()),
      Expr{false}, [interpreter](Expr&& acc, Expr&& expr) -> Expr {
        return IsTruthy(acc) ? acc : interpreter->Evaluate(std::move(expr));
      });
}

Expr And::Call(Interpreter* interpreter, args_type&& exprs) {
  return std::accumulate(
      make_move_iterator(exprs.begin()), make_move_iterator(exprs.end()),
      Expr{true}, [interpreter](Expr&& acc, Expr&& expr) -> Expr {
        return IsTruthy(acc) ? interpreter->Evaluate(std::move(expr)) : acc;
      });
}

}  // namespace built_in
}  // namespace simpl
