// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/control_flow.hh"

#include <iterator>
#include <memory>
#include <utility>

#include "simpl/ast.hh"
#include "simpl/interpreter.hh"
#include "simpl/interpreter_util.hh"

namespace simpl {

namespace built_in {

Expr If::Call(Interpreter* interpreter, args_type&& exprs) {
  if (exprs.size() != 3) {
    throw std::runtime_error("'if' expects three arguments");
  }

  auto i = make_move_iterator(exprs.begin());
  auto cond = interpreter->Evaluate(*i++);
  auto then = i++;
  auto otherwise = i++;

  if (IsTruthy(cond)) {
    return interpreter->Evaluate(*then);
  } else {
    return interpreter->Evaluate(*otherwise);
  }
}

Expr Let::Call(Interpreter* interpreter, args_type&& exprs) {
  auto i = exprs.begin();
  auto bindings = std::get<Vector>(*i++);
  if (bindings.size() % 2 != 0) {
    throw std::runtime_error(
        "The number of expressions in the binding list must be even.");
  }
  auto env = std::make_shared<Interpreter::Environment>(interpreter->env());
  for (auto j = make_move_iterator(bindings.begin());
       j != make_move_iterator(bindings.end());) {
    auto name = std::get<Symbol>(*j++).name;
    auto value = interpreter->Evaluate(*j++);
    env->Bind(name, value);
  }
  return interpreter->Evaluate(ExprList(i, exprs.end()), env);
}

Expr Do::FnCall(Interpreter*, args_type&& args) {
  return std::move(args.back());
}

}  // namespace built_in

}  // namespace simpl
