// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_USER_FN_H_
#define SIMPL_USER_FN_H_

#include "simpl/callable.h"
#include "simpl/interpreter.h"
#include "simpl/parser.h"

namespace simpl {

class UserFn : public Callable {
 public:
  int arity() const override { return definition_.params().size(); }
  explicit UserFn(const Expr::Fn& definition) : definition_(definition) {}

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override;
  const Expr::Fn& definition_;
};

}  // namespace simpl

#endif  // SIMPL_USER_FN_H_
