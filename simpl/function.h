// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_FUNCTION_H_
#define SIMPL_FUNCTION_H_

#include "simpl/ast.h"
#include "simpl/callable.h"

namespace simpl {

class Function : public Callable {
 public:
  using args_type = std::list<Expr>;
  Expr Call(Interpreter*, const expr_list_t& exprs) override;

 private:
  virtual Expr FnCall(Interpreter*, const args_type& args) = 0;
};

}  // namespace simpl

#endif  // SIMPL_FUNCTION_H_
