// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/control_flow.h"

#include "simpl/ast.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

Expr::Atom::value_type If::Call(Interpreter* interpreter,
                                const expr_list_t& exprs) {
  if (exprs.size() != 3) {
    throw std::runtime_error("'if' expects three arguments");
  }

  auto i = exprs.begin();
  auto cond = interpreter->Evaluate(**i++);
  auto then = *i++;
  auto otherwise = *i++;

  if (IsTruthy(cond)) {
    return interpreter->Evaluate(*then);
  } else {
    return interpreter->Evaluate(*otherwise);
  }
}

}  // namespace built_in

}  // namespace simpl
