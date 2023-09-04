// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/callable.hh"

#include <ostream>

#include "simpl/ast.hh"

namespace simpl {

// TODO(hjiang): remove this
Expr Callable::Call(Interpreter*, args_iterator begin, args_iterator end) {
  return Call(nullptr, args_type(begin, end));
}

std::ostream& operator<<(std::ostream& os, const Callable&) {
  return os << "<callable>";
}

}  // namespace simpl
