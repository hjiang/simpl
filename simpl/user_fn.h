// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_USER_FN_H_
#define SIMPL_USER_FN_H_

#include <list>
#include <memory>
#include <string>
#include <utility>

#include "simpl/ast.h"
#include "simpl/function.h"
#include "simpl/interpreter.h"

namespace simpl {

class FnDef {
 public:
  using param_list_t = std::list<std::string>;
  using body_t = ExprList;
  FnDef(param_list_t&& params, body_t&& body, std::string&& param_rest = "")
      : params_(std::move(params)),
        param_rest_(std::move(param_rest)),
        body_(std::move(body)) {}
  virtual ~FnDef() = default;
  const param_list_t& params() const { return params_; }
  const std::string& param_rest() const { return param_rest_; }
  const body_t& body() const { return body_; }

 private:
  const param_list_t params_;
  const std::string param_rest_;
  const body_t body_;
};

class UserFn : public Function {
 public:
  explicit UserFn(FnDef&& definition,
                  std::shared_ptr<Interpreter::Environment> closure = nullptr,
                  bool lazy = false)
      : Function(lazy), definition_(definition), closure_(closure) {}

 private:
  Expr FnCall(Interpreter*, const args_type& args) override;
  FnDef definition_;
  std::shared_ptr<Interpreter::Environment> closure_;
};

}  // namespace simpl

#endif  // SIMPL_USER_FN_H_
