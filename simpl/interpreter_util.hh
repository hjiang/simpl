// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_INTERPRETER_UTIL_HH_
#define SIMPL_INTERPRETER_UTIL_HH_

#include <cstddef>
#include <string>
#include <variant>

#include "simpl/interpreter.hh"

namespace simpl {

template <typename T>
bool holds(Expr value) {
  return std::holds_alternative<T>(value);
}

bool IsTruthy(const Expr& v);

template <typename T>
std::string TypeStr();

template <>
inline std::string TypeStr<int_type>() {
  return "int_type";
}

template <>
inline std::string TypeStr<float_type>() {
  return "float_type";
}

template <>
inline std::string TypeStr<Symbol>() {
  return "Symbol";
}

template <>
inline std::string TypeStr<std::string>() {
  return "std::string";
}

template <>
inline std::string TypeStr<std::nullptr_t>() {
  return "std::nullptr_t";
}

template <>
inline std::string TypeStr<bool>() {
  return "bool";
}

}  // namespace simpl

#endif  // SIMPL_INTERPRETER_UTIL_HH_
