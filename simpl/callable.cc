// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/callable.h"

namespace simpl {

Interpreter::atom_value_type Callable::Call(Interpreter* interpreter,
                                            const args_type& args) {
  auto n = arity();
  if (n >= 0 && args.size() != static_cast<size_t>(n)) {
    throw std::runtime_error("Expecting " + std::to_string(arity()) +
                             " arguments, got " + std::to_string(args.size()) +
                             ".");
  }
  return CallImpl(interpreter, args);
}

}  // namespace simpl
