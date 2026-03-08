// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/macro.hh"

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "simpl/ast.hh"
#include "simpl/interpreter.hh"
#include "simpl/user_macro.hh"

namespace simpl {
namespace built_in {

Expr Defmacro::Call(Interpreter* interpreter, args_type&& exprs) {
  if (exprs.size() < 3) {
    throw std::runtime_error("`defmacro` expects at least 3 arguments.");
  }
  auto name = std::get<Symbol>(exprs.front()).name;

  // Reuse the same param-parsing logic as fn: build (fn [params] body...)
  // and extract params + body manually.
  namespace rgs = std::ranges;
  auto it = exprs.begin();
  ++it;  // skip name
  auto params = std::get<Vector>(*it);
  ++it;  // skip params

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

  FnDef::body_t body;
  std::move(it, exprs.end(), std::back_inserter(body));

  auto macro = std::make_shared<UserMacro>(
      FnDef(std::move(param_names), std::move(body), std::move(param_rest)),
      interpreter->env());
  interpreter->env()->Define(name, Expr{callable_ptr_t{macro}});
  return Expr{nullptr};
}

Expr Macroexpand::FnCall(Interpreter* interpreter, args_type&& args) {
  if (args.size() != 1) {
    throw std::runtime_error("`macroexpand` expects 1 argument.");
  }
  auto form = std::get<List>(std::move(args.front()));
  if (form.empty()) {
    return Expr{std::move(form)};
  }
  auto first = std::get<Symbol>(form.front()).name;
  auto resolved = interpreter->env()->Get(first);
  auto callable = std::get<callable_ptr_t>(resolved);
  auto macro = std::dynamic_pointer_cast<UserMacro>(callable);
  if (!macro) {
    throw std::runtime_error(
        "`macroexpand`: first element is not a macro.");
  }
  auto it = form.begin();
  ++it;
  ExprList macro_args;
  std::move(it, form.end(), std::back_inserter(macro_args));
  return macro->Call(interpreter, std::move(macro_args));
}

}  // namespace built_in
}  // namespace simpl
