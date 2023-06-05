// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_USER_FN_H_
#define SIMPL_USER_FN_H_

#include <list>
#include <memory>
#include <string>
#include <utility>

#include "simpl/ast.h"
#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

class FnDef {
 public:
  using param_list_t = std::list<std::string>;
  using body_t = expr_list_t;
  FnDef(param_list_t&& params, body_t&& body)
      : params_(std::move(params)), body_(std::move(body)) {}
  virtual ~FnDef() = default;
  const param_list_t& params() const { return params_; }
  const body_t& body() const { return body_; }

 private:
  const param_list_t params_;
  const body_t body_;
};

class UserFn : public Function {
 public:
  UserFn(FnDef&& definition,
         std::shared_ptr<Interpreter::Environment> closure = nullptr)
      : definition_(definition), closure_(closure) {}

 private:
  Expr FnCall(Interpreter*, const args_type& args) override;
  FnDef definition_;
  std::shared_ptr<Interpreter::Environment> closure_;
};

}  // namespace simpl

#endif  // SIMPL_USER_FN_H_
