#include "arithmetic.h"

#include "interpreter_util.h"

namespace oxid {

Interpreter::atom_value_type operator+(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<long>(lhs) && std::holds_alternative<long>(rhs)) {
    return std::get<long>(lhs) + std::get<long>(rhs);
  }
  if (std::holds_alternative<long>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<long>(lhs) + std::get<double>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<long>(rhs)) {
    return std::get<double>(lhs) + std::get<long>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<double>(lhs) + std::get<double>(rhs);
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
  if (std::holds_alternative<long>(lhs) && std::holds_alternative<long>(rhs)) {
    return std::get<long>(lhs) - std::get<long>(rhs);
  }
  if (std::holds_alternative<long>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<long>(lhs) - std::get<double>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<long>(rhs)) {
    return std::get<double>(lhs) - std::get<long>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<double>(lhs) - std::get<double>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator -");  // FIXME: error handling
}

Interpreter::atom_value_type operator*(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<long>(lhs) && std::holds_alternative<long>(rhs)) {
    return std::get<long>(lhs) * std::get<long>(rhs);
  }
  if (std::holds_alternative<long>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<long>(lhs) * std::get<double>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<long>(rhs)) {
    return std::get<double>(lhs) * std::get<long>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<double>(lhs) * std::get<double>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator *");  // FIXME: error handling
}

Interpreter::atom_value_type operator/(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<long>(lhs) && std::holds_alternative<long>(rhs)) {
    return std::get<long>(lhs) / std::get<long>(rhs);
  }
  if (std::holds_alternative<long>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<long>(lhs) / std::get<double>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<long>(rhs)) {
    return std::get<double>(lhs) / std::get<long>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<double>(lhs) / std::get<double>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator /");  // FIXME: error handling
}

Interpreter::atom_value_type operator%(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<long>(lhs) && std::holds_alternative<long>(rhs)) {
    return std::get<long>(lhs) % std::get<long>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator %");  // FIXME: error handling
}

}  // namespace oxid
