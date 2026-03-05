// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_FUNCTION_HH_
#define SIMPL_FUNCTION_HH_

#include "simpl/ast.hh"
#include "simpl/callable.hh"

namespace simpl {

class Function : public Callable {
 public:
  explicit Function(bool lazy = false) : lazy_(lazy) {}
  Expr Call(Interpreter*, args_type&& exprs) override;

 private:
  virtual Expr FnCall(Interpreter*, args_type&& args) = 0;
  bool lazy_;
};

}  // namespace simpl

#endif  // SIMPL_FUNCTION_HH_
