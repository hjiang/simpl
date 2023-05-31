// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_CONTROL_FLOW_H_
#define SIMPL_BUILT_IN_CONTROL_FLOW_H_

#include "simpl/ast.h"
#include "simpl/callable.h"

namespace simpl {

namespace built_in {

class If : public Callable {
 public:
  virtual ~If() {}
  Expr::Atom::value_type Call(Interpreter* interpreter,
                              const expr_list_t& exprs) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_CONTROL_FLOW_H_
