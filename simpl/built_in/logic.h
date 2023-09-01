// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_LOGIC_H_
#define SIMPL_BUILT_IN_LOGIC_H_

#include "simpl/callable.h"
#include "simpl/function.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

class Not : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override {
    return Expr(static_cast<bool>(!IsTruthy(args.front())));
  }
};

class Or : public Callable {
 public:
  virtual ~Or() = default;
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;
};

class And : public Callable {
 public:
  virtual ~And() = default;
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_LOGIC_H_
