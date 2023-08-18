// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_FUNCTION_H_
#define SIMPL_FUNCTION_H_

#include <list>

#include "simpl/ast.h"
#include "simpl/callable.h"

namespace simpl {

class Function : public Callable {
 public:
  explicit Function(bool lazy = false) : lazy_(lazy) {}
  using args_type = std::list<Expr>;
  Expr Call(Interpreter*, const ExprList& exprs) override;

 private:
  virtual Expr FnCall(Interpreter*, const args_type& args) = 0;
  bool lazy_;
};

}  // namespace simpl

#endif  // SIMPL_FUNCTION_H_
