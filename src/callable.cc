// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/callable.h"

#include <ostream>

#include "simpl/ast.h"

namespace simpl {

std::ostream& operator<<(std::ostream& os, const Callable&) {
  return os << "<callable>";
}

}  // namespace simpl
