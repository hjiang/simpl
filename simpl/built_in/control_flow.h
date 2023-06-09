// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_CONTROL_FLOW_H_
#define SIMPL_BUILT_IN_CONTROL_FLOW_H_

#include "simpl/ast.h"
#include "simpl/callable.h"
#include "simpl/function.h"

namespace simpl {

namespace built_in {

class If : public Callable {
 public:
  virtual ~If() = default;
  Expr Call(Interpreter* interpreter, const expr_list_t& exprs) override;
};

class Do : public Function {
 public:
  virtual ~Do() = default;

 private:
  Expr FnCall(Interpreter*, const args_type& args) override {
    return args.back();
  };
};

class Let : public Callable {
 public:
  virtual ~Let() = default;
  Expr Call(Interpreter* interpreter, const expr_list_t& exprs) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_CONTROL_FLOW_H_
