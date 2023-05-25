// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/comparison.h"

#include <compare>
#include <list>
#include <string>
#include <variant>

#include "simpl/interpreter.h"

namespace simpl {

namespace builtin_fn {

static std::partial_ordering Compare(
    const std::list<Interpreter::atom_value_type>& args) {
  if (args.size() != 2) {
    throw std::runtime_error("comparison requires 2 arguments");
  }
  auto lhs = args.front();
  auto rhs = args.back();
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<int_type>(lhs) <=> std::get<int_type>(rhs);
  }
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<int_type>(lhs) <=> std::get<float_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<float_type>(lhs) <=> std::get<int_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<float_type>(lhs) <=> std::get<double>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator +");  // FIXME: error handling
}

Interpreter::atom_value_type Equals::CallImpl(Interpreter*,
                                              const args_type& args) {
  return Compare(args) == std::partial_ordering::equivalent;
}

Interpreter::atom_value_type GreaterThan::CallImpl(Interpreter*,
                                                   const args_type& args) {
  return Compare(args) == std::partial_ordering::greater;
}

Interpreter::atom_value_type GreaterThanOrEqualTo::CallImpl(
    Interpreter*, const args_type& args) {
  return Compare(args) == std::partial_ordering::greater ||
         Compare(args) == std::partial_ordering::equivalent;
}

Interpreter::atom_value_type LessThan::CallImpl(Interpreter*,
                                                const args_type& args) {
  return Compare(args) == std::partial_ordering::less;
}

Interpreter::atom_value_type LessThanOrEqualTo::CallImpl(
    Interpreter*, const args_type& args) {
  return Compare(args) == std::partial_ordering::less ||
         Compare(args) == std::partial_ordering::equivalent;
}

}  // namespace builtin_fn

}  // namespace simpl
