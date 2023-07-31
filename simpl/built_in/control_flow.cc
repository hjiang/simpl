// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/control_flow.h"

#include <memory>

#include "simpl/ast.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

Expr If::Call(Interpreter* interpreter, const expr_list_t& exprs) {
  if (exprs.size() != 3) {
    throw std::runtime_error("'if' expects three arguments");
  }

  auto i = exprs.begin();
  auto cond = interpreter->Evaluate(*i++);
  auto then = i++;
  auto otherwise = i++;

  if (IsTruthy(cond)) {
    return interpreter->Evaluate(*then);
  } else {
    return interpreter->Evaluate(*otherwise);
  }
}

Expr Let::Call(Interpreter* interpreter, const expr_list_t& exprs) {
  auto i = exprs.begin();
  auto bindings = std::get<Vector>(*i++).exprs();
  if (bindings.size() % 2 != 0) {
    throw std::runtime_error(
        "The number of expressions in the binding list must be even.");
  }
  auto env = std::make_shared<Interpreter::Environment>(interpreter->env());
  for (auto j = bindings.begin(); j != bindings.end();) {
    auto name = std::get<Symbol>(*j++).name;
    auto value = interpreter->Evaluate(*j++);
    env->Bind(name, value);
  }
  return interpreter->Evaluate(expr_list_t(i, exprs.end()), env);
}

}  // namespace built_in

}  // namespace simpl
