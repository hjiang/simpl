// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/comparison.h"

#include <compare>
#include <list>
#include <string>
#include <variant>

#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

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
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return OrderingToInt(std::get<int_type>(lhs) <=> std::get<int_type>(rhs));
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return OrderingToInt(std::get<int_type>(lhs) <=> std::get<float_type>(rhs));
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return OrderingToInt(std::get<float_type>(lhs) <=> std::get<int_type>(rhs));
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return OrderingToInt(std::get<float_type>(lhs) <=> std::get<double>(rhs));
  }
  if (holds<std::string>(lhs) && holds<std::string>(rhs)) {
    return std::get<std::string>(lhs).compare(std::get<std::string>(rhs));
  }
  throw std::runtime_error("Incomparable types");  // FIXME: error handling
}

}  // namespace

Interpreter::atom_value_type Equals::FnCall(Interpreter*,
                                                const args_type& args) {
  return Compare(args) == 0;
}

Interpreter::atom_value_type GreaterThan::FnCall(Interpreter*,
                                                     const args_type& args) {
  return Compare(args) > 0;
}

Interpreter::atom_value_type GreaterThanOrEqualTo::FnCall(
    Interpreter*, const args_type& args) {
  return Compare(args) >= 0;
}

Interpreter::atom_value_type LessThan::FnCall(Interpreter*,
                                                  const args_type& args) {
  return Compare(args) < 0;
}

Interpreter::atom_value_type LessThanOrEqualTo::FnCall(
    Interpreter*, const args_type& args) {
  return Compare(args) <= 0;
}

}  // namespace built_in

}  // namespace simpl
