// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/def.hh"

#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

#include "simpl/ast.hh"
#include "simpl/interpreter.hh"

namespace simpl {
namespace built_in {

Expr Def::Call(Interpreter* interpreter, args_type&& exprs) {
  if (exprs.size() != 2) {
    throw std::runtime_error("`def` expects 2 arguments.");
  }
  interpreter->env()->Define(std::get<Symbol>(exprs.front()).name,
                             interpreter->Evaluate(std::move(exprs.back())));
  return Expr{nullptr};
}

Expr Defn::Call(Interpreter* interpreter, args_type&& exprs) {
  if (exprs.size() < 2) {
    throw std::runtime_error("`defn` expects at least 2 arguments.");
  }
  auto name = std::get<Symbol>(exprs.front()).name;
  List fn_body(++exprs.begin(), exprs.end());
  fn_body.push_front(Expr{Symbol{"fn"}});
  interpreter->env()->Define(name, interpreter->Evaluate(std::move(fn_body)));
  return Expr{nullptr};
}

}  // namespace built_in
}  // namespace simpl
