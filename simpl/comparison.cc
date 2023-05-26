// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/comparison.h"

#include <compare>
#include <list>
#include <string>
#include <variant>

#include "simpl/interpreter.h"

namespace simpl {

namespace builtin_fn {

namespace {

int OrderingToInt(std::partial_ordering ordering) {
  if (ordering == std::partial_ordering::less) {
    return -1;
  }
  if (ordering == std::partial_ordering::equivalent) {
    return 0;
  }
  if (ordering == std::partial_ordering::greater) {
    return 1;
  }
  throw std::runtime_error("Invalid ordering");
}

int Compare(const std::list<Interpreter::atom_value_type>& args) {
  if (args.size() != 2) {
    throw std::runtime_error("comparison requires 2 arguments");
  }
  auto lhs = args.front();
  auto rhs = args.back();
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return OrderingToInt(std::get<int_type>(lhs) <=> std::get<int_type>(rhs));
  }
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return OrderingToInt(std::get<int_type>(lhs) <=> std::get<float_type>(rhs));
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return OrderingToInt(std::get<float_type>(lhs) <=> std::get<int_type>(rhs));
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return OrderingToInt(std::get<float_type>(lhs) <=> std::get<double>(rhs));
  }
  if (std::holds_alternative<std::string>(lhs) &&
      std::holds_alternative<std::string>(rhs)) {
    return std::get<std::string>(lhs).compare(std::get<std::string>(rhs));
  }
  throw std::runtime_error("Incomparable types");  // FIXME: error handling
}

}  // namespace

Interpreter::atom_value_type Equals::CallImpl(Interpreter*,
                                              const args_type& args) {
  return Compare(args) == 0;
}

Interpreter::atom_value_type GreaterThan::CallImpl(Interpreter*,
                                                   const args_type& args) {
  return Compare(args) > 0;
}

Interpreter::atom_value_type GreaterThanOrEqualTo::CallImpl(
    Interpreter*, const args_type& args) {
  return Compare(args) >= 0;
}

Interpreter::atom_value_type LessThan::CallImpl(Interpreter*,
                                                const args_type& args) {
  return Compare(args) < 0;
}

Interpreter::atom_value_type LessThanOrEqualTo::CallImpl(
    Interpreter*, const args_type& args) {
  return Compare(args) <= 0;
}

}  // namespace builtin_fn

}  // namespace simpl
