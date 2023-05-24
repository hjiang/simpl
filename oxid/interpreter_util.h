// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef INTERPRETER_UTIL_H_
#define INTERPRETER_UTIL_H_

#include <cstddef>
#include <string>
#include <variant>

#include "oxid/interpreter.h"

namespace oxid {

template <typename T>
bool holds(Interpreter::atom_value_type value) {
  return std::holds_alternative<T>(value);
}

bool IsTruthy(const Interpreter::atom_value_type& v);

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
inline std::string TypeStr<Expr::Symbol>() {
  return "Expr::Symbol";
}

template <>
inline std::string TypeStr<Interpreter::Symbol>() {
  return "Interpreter::Symbol";
}

template <>
inline std::string TypeStr<std::string>() {
  return "std::string";
}

template <>
inline std::string TypeStr<nullptr_t>() {
  return "nullptr_t";
}

template <>
inline std::string TypeStr<bool>() {
  return "bool";
}

}  // namespace oxid

#endif  // INTERPRETER_UTIL_H_
