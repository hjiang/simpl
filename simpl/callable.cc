// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/callable.h"

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {

Interpreter::atom_value_type Function::Call(Interpreter* interpreter,
                                            const expr_list_t& exprs) {
  args_type args;
  for (auto& expr : exprs) {
    args.push_back(interpreter->Evaluate(*expr));
  }
  return FnCall(interpreter, args);
}

}  // namespace simpl
