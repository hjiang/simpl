#ifndef INTERPRETER_UTIL_H_
#define INTERPRETER_UTIL_H_

#include <cstddef>
#include <string>
#include <variant>

#include "interpreter.h"

namespace oxid {

template <typename T>
bool holds(Interpreter::atom_value_type value) {
  return std::holds_alternative<T>(value);
}

bool IsTruthy(const Interpreter::atom_value_type& v);

template <typename T>
std::string TypeStr();

template <>
inline std::string TypeStr<long>() {
  return "long";
}

template <>
inline std::string TypeStr<double>() {
  return "double";
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
