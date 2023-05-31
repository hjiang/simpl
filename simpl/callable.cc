// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/callable.h"

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {

Interpreter::atom_value_type Callable::Call(Interpreter* interpreter,
                                            const expr_list_t& exprs) {
  auto n = arity();
  if (n >= 0 && exprs.size() != static_cast<size_t>(n)) {
    throw std::runtime_error("Expecting " + std::to_string(arity()) +
                             " arguments, got " + std::to_string(exprs.size()) +
                             ".");
  }
  return CallImpl(interpreter, exprs);
}

Interpreter::atom_value_type Function::CallImpl(Interpreter* interpreter,
                                                const expr_list_t& exprs) {
  args_type args;
  for (auto& expr : exprs) {
    args.push_back(interpreter->Evaluate(*expr));
  }
  return FnCallImpl(interpreter, args);
}

}  // namespace simpl
