// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/def.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {
namespace built_in {

Expr Def::Call(Interpreter* interpreter, const expr_list_t& exprs) {
  if (exprs.size() != 2) {
    throw std::runtime_error("`def` expects 2 arguments.");
  }
  interpreter->env()->Define(std::get<Symbol>(exprs.front()).name,
                             interpreter->Evaluate(exprs.back()));
  return interpreter->last_value();
}

Expr Defn::Call(Interpreter* interpreter, const expr_list_t& exprs) {
  if (exprs.size() < 2) {
    throw std::runtime_error("`defn` expects at least 2 arguments.");
  }
  auto name = std::get<Symbol>(exprs.front()).name;
  expr_list_t fn_body(++exprs.begin(), exprs.end());
  fn_body.push_front(Expr{Symbol{"fn"}});
  interpreter->env()->Define(name,
                             interpreter->Evaluate(List{std::move(fn_body)}));
  return interpreter->last_value();
}

}  // namespace built_in
}  // namespace simpl
