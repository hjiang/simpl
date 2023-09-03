// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_FUNCTION_H_
#define SIMPL_FUNCTION_H_

#include "simpl/ast.h"
#include "simpl/callable.h"

namespace simpl {

class Function : public Callable {
 public:
  explicit Function(bool lazy = false) : lazy_(lazy) {}
  Expr Call(Interpreter*, args_type&& exprs) override;
  Expr Call(Interpreter*, args_iterator begin, args_iterator end) override;

 private:
  virtual Expr FnCall(Interpreter*, args_type&& args) = 0;
  virtual Expr FnCall(Interpreter*, args_iterator begin, args_iterator end);
  bool lazy_;
};

}  // namespace simpl

#endif  // SIMPL_FUNCTION_H_
