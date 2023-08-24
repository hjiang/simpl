// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/fn.h"

#include <algorithm>
#include <memory>
#include <ranges>
#include <string>
#include <utility>

#include "simpl/ast.h"
#include "simpl/user_fn.h"

namespace simpl {
namespace built_in {

Expr Fn::Call(Interpreter* interpreter, const ExprList& exprs) {
  namespace rgs = std::ranges;
  if (exprs.size() < 1) {
    throw std::runtime_error("fn: missing arguments");
  }
  auto i = exprs.begin();
  auto params = std::get<Vector>(*i++);
  auto pos_params_end = rgs::find_if(params, [](const auto& param) {
    return std::get<Symbol>(param).name == "&";
  });
  FnDef::param_list_t param_names;
  rgs::transform(
      params.begin(), pos_params_end, std::back_inserter(param_names),
      [](const auto& param) { return std::get<Symbol>(param).name; });
  std::string param_rest;
  if (pos_params_end != params.end()) {
    param_rest = std::get<Symbol>(*++pos_params_end).name;
  }
  FnDef::body_t body(i, exprs.end());
  return std::make_unique<UserFn>(
      FnDef(FnDef::param_list_t(param_names.begin(), param_names.end()),
            std::move(body), std::move(param_rest)),
      interpreter->env(), lazy_);
}

}  // namespace built_in
}  // namespace simpl
