// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_CONTROL_FLOW_HH_
#define SIMPL_BUILT_IN_CONTROL_FLOW_HH_

#include "simpl/ast.hh"
#include "simpl/callable.hh"
#include "simpl/function.hh"

namespace simpl {

namespace built_in {

class If : public Callable {
 public:
  virtual ~If() = default;
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;
};

class Do : public Function {
 public:
  virtual ~Do() = default;

 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class Let : public Callable {
 public:
  virtual ~Let() = default;
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_CONTROL_FLOW_HH_
