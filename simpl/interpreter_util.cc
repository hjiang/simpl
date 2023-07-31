// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter_util.h"

namespace simpl {

bool IsTruthy(const Expr& v) {
  if (holds<bool>(v)) {
    return std::get<bool>(v);
  } else if (holds<std::nullptr_t>(v)) {
    return false;
  } else {
    return true;
  }
}

}  // namespace simpl
