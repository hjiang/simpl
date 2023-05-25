// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/comparison.h"

#include <string>

namespace simpl {

Interpreter::atom_value_type Equal(
    const std::vector<Interpreter::atom_value_type>& args) {
  if (args.size() != 2) {
    throw std::runtime_error("'=' takes 2 arguments");
  }
  auto lhs = args[0];
  auto rhs = args[1];
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<int_type>(lhs) == std::get<int_type>(rhs);
  }
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<int_type>(lhs) == std::get<float_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<float_type>(lhs) == std::get<int_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<float_type>(lhs) == std::get<double>(rhs);
  }
  if (std::holds_alternative<std::string>(lhs) &&
      std::holds_alternative<std::string>(rhs)) {
    return std::get<std::string>(lhs) == std::get<std::string>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator +");  // FIXME: error handling
}

}  // namespace simpl
