// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/arithmetic.h"

#include <string>

#include "simpl/interpreter_util.h"

namespace simpl {

Interpreter::atom_value_type operator+(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<int_type>(lhs) + std::get<int_type>(rhs);
  }
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<int_type>(lhs) + std::get<float_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<float_type>(lhs) + std::get<int_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<float_type>(lhs) + std::get<double>(rhs);
  }
  if (std::holds_alternative<std::string>(lhs) &&
      std::holds_alternative<std::string>(rhs)) {
    return std::get<std::string>(lhs) + std::get<std::string>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator +");  // FIXME: error handling
}

Interpreter::atom_value_type operator-(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<int_type>(lhs) - std::get<int_type>(rhs);
  }
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<int_type>(lhs) - std::get<float_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<float_type>(lhs) - std::get<int_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<float_type>(lhs) - std::get<double>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator -");  // FIXME: error handling
}

Interpreter::atom_value_type operator*(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<int_type>(lhs) * std::get<int_type>(rhs);
  }
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<int_type>(lhs) * std::get<float_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<float_type>(lhs) * std::get<int_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<float_type>(lhs) * std::get<double>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator *");  // FIXME: error handling
}

Interpreter::atom_value_type operator/(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<int_type>(lhs) / std::get<int_type>(rhs);
  }
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<int_type>(lhs) / std::get<float_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<float_type>(lhs) / std::get<int_type>(rhs);
  }
  if (std::holds_alternative<float_type>(lhs) &&
      std::holds_alternative<float_type>(rhs)) {
    return std::get<float_type>(lhs) / std::get<double>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator /");  // FIXME: error handling
}

Interpreter::atom_value_type operator%(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<int_type>(lhs) &&
      std::holds_alternative<int_type>(rhs)) {
    return std::get<int_type>(lhs) % std::get<int_type>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator %");  // FIXME: error handling
}

}  // namespace simpl