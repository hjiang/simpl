// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/def.h"

#include <memory>
#include <stdexcept>
#include <variant>

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {
namespace built_in {

Expr::Atom::value_type Def::Call(Interpreter* interpreter,
                                 const expr_list_t& exprs) {
  if (exprs.size() != 2) {
    throw std::runtime_error("`def` expects 2 arguments.");
  }
  auto atom = std::dynamic_pointer_cast<const Expr::Atom>(exprs.front());
  interpreter->env()->Define(atom->value<Expr::Symbol>().name,
                             interpreter->Evaluate(*exprs.back()));
  return interpreter->last_value();
}

Expr::Atom::value_type Defn::Call(Interpreter* interpreter,
                                  const expr_list_t& exprs) {
  if (exprs.size() < 2) {
    throw std::runtime_error("`defn` expects at least 2 arguments.");
  }
  auto atom = std::dynamic_pointer_cast<const Expr::Atom>(exprs.front());
  expr_list_t fn_body(++exprs.begin(), exprs.end());
  fn_body.push_front(std::make_unique<Expr::Atom>(Expr::Symbol{"fn"}));
  interpreter->env()->Define(
      atom->value<Expr::Symbol>().name,
      interpreter->Evaluate(Expr::List(std::move(fn_body))));
  return interpreter->last_value();
}

}  // namespace built_in
}  // namespace simpl
