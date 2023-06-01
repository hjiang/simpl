// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/fn.h"

#include <iostream>
#include <memory>
#include <utility>

#include "simpl/ast.h"
#include "simpl/user_fn.h"

namespace simpl {
namespace built_in {

Expr::Atom::value_type Fn::Call(Interpreter* interpreter,
                                const expr_list_t& exprs) {
  if (exprs.size() < 1) {
    throw std::runtime_error("fn: missing arguments");
  }
  auto i = exprs.begin();
  auto params = dynamic_pointer_cast<const Expr::Vector>(*i++);
  FnDef::param_list_t param_list;

  for (const auto& param : params->exprs()) {
    auto atom = dynamic_pointer_cast<const Expr::Atom>(param);
    param_list.push_back(atom->value<Expr::Symbol>().name);
    std::cerr << "arg " << atom->value<Expr::Symbol>().name << std::endl;
  }
  FnDef::body_t body(i, exprs.end());
  return std::make_unique<UserFn>(FnDef(std::move(param_list), std::move(body)),
                                  interpreter->env());
}

}  // namespace built_in
}  // namespace simpl
