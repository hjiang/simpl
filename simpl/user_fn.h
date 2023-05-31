// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_USER_FN_H_
#define SIMPL_USER_FN_H_

#include <memory>

#include "simpl/ast.h"
#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

class UserFn : public Function {
 public:
  int arity() const override { return definition_.params().size(); }
  UserFn(const Expr::Fn& definition,
         std::shared_ptr<Interpreter::Environment> closure = nullptr)
      : definition_(definition), closure_(closure) {}

 private:
  Interpreter::atom_value_type FnCallImpl(Interpreter*,
                                          const args_type& args) override;
  const Expr::Fn& definition_;
  std::shared_ptr<Interpreter::Environment> closure_;
};

}  // namespace simpl

#endif  // SIMPL_USER_FN_H_
