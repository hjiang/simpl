#ifndef INTERPRETER_UTIL_H_
#define INTERPRETER_UTIL_H_

#include <variant>

#include "interpreter.h"

namespace oxid {

template <typename T>
bool holds(Interpreter::atom_value_type value) {
  return std::holds_alternative<T>(value);
}

}  // namespace oxid

#endif  // INTERPRETER_UTIL_H_
