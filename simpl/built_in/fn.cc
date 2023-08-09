// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/fn.h"

#include <memory>
#include <ranges>
#include <utility>

#include "simpl/ast.h"
#include "simpl/user_fn.h"

namespace simpl {
namespace built_in {

Expr Fn::Call(Interpreter* interpreter, const ExprList& exprs) {
  namespace rvs = std::ranges::views;
  if (exprs.size() < 1) {
    throw std::runtime_error("fn: missing arguments");
  }
  auto i = exprs.begin();
  auto params = std::get<Vector>(*i++);
  auto param_names = rvs::transform(
      params, [](const auto& param) { return std::get<Symbol>(param).name; });
  FnDef::body_t body(i, exprs.end());
  return std::make_unique<UserFn>(
      FnDef(FnDef::param_list_t(param_names.begin(), param_names.end()),
            std::move(body)),
      interpreter->env());
}

}  // namespace built_in
}  // namespace simpl
