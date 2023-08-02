// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/fn.h"

#include <memory>
#include <utility>

#include "simpl/ast.h"
#include "simpl/user_fn.h"

namespace simpl {
namespace built_in {

Expr Fn::Call(Interpreter* interpreter, const ExprList& exprs) {
  if (exprs.size() < 1) {
    throw std::runtime_error("fn: missing arguments");
  }
  auto i = exprs.begin();
  auto params = std::get<Vector>(*i++);
  FnDef::param_list_t param_list;

  for (const auto& param : params) {
    param_list.push_back(std::get<Symbol>(param).name);
  }
  FnDef::body_t body(i, exprs.end());
  return std::make_unique<UserFn>(FnDef(std::move(param_list), std::move(body)),
                                  interpreter->env());
}

}  // namespace built_in
}  // namespace simpl
