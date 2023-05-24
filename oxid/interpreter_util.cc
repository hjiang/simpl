// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "interpreter_util.h"

namespace oxid {

bool IsTruthy(const Interpreter::atom_value_type& v) {
  if (holds<bool>(v)) {
    return std::get<bool>(v);
  } else if (holds<nullptr_t>(v)) {
    return false;
  } else {
    return true;
  }
}

}  // namespace oxid
