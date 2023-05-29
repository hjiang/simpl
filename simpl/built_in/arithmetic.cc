// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/arithmetic.h"

#include <string>

#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

static Interpreter::atom_value_type operator+(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<int_type>(lhs) + std::get<int_type>(rhs);
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<int_type>(lhs) + std::get<float_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<float_type>(lhs) + std::get<int_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<float_type>(lhs) + std::get<float_type>(rhs);
  }
  if (holds<std::string>(lhs) && holds<std::string>(rhs)) {
    return std::get<std::string>(lhs) + std::get<std::string>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator +");  // FIXME: error handling
}

static Interpreter::atom_value_type operator-(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<int_type>(lhs) - std::get<int_type>(rhs);
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<int_type>(lhs) - std::get<float_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<float_type>(lhs) - std::get<int_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<float_type>(lhs) - std::get<float_type>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator -");  // FIXME: error handling
}

static Interpreter::atom_value_type operator*(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<int_type>(lhs) * std::get<int_type>(rhs);
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<int_type>(lhs) * std::get<float_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<float_type>(lhs) * std::get<int_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<float_type>(lhs) * std::get<float_type>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator *");  // FIXME: error handling
}

static Interpreter::atom_value_type operator/(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<int_type>(lhs) / std::get<int_type>(rhs);
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<int_type>(lhs) / std::get<float_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<float_type>(lhs) / std::get<int_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return std::get<float_type>(lhs) / std::get<float_type>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator /");  // FIXME: error handling
}

static Interpreter::atom_value_type operator%(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<int_type>(lhs) % std::get<int_type>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator %");  // FIXME: error handling
}

Interpreter::atom_value_type Sum::CallImpl(Interpreter*,
                                           const args_type& args) {
  Interpreter::atom_value_type result = args.front();
  auto i = args.begin();
  ++i;
  for (; i != args.end(); ++i) {
    result = result + *i;
  }
  return result;
}

Interpreter::atom_value_type Substract::CallImpl(Interpreter*,
                                                 const args_type& args) {
  auto i = args.begin();
  Interpreter::atom_value_type result = *i++;
  for (; i != args.end(); ++i) {
    result = result - *i;
  }
  return args.size() > 1 ? result : 0 - result;
}
Interpreter::atom_value_type Multiply::CallImpl(Interpreter*,
                                                const args_type& args) {
  Interpreter::atom_value_type result = 1;
  for (auto arg : args) {
    result = result * arg;
  }
  return result;
}
Interpreter::atom_value_type Divide::CallImpl(Interpreter*,
                                              const args_type& args) {
  auto i = args.begin();
  Interpreter::atom_value_type result = *i++;
  for (; i != args.end(); ++i) {
    result = result / *i;
  }
  return result;
}
Interpreter::atom_value_type Modulo::CallImpl(Interpreter*,
                                              const args_type& args) {
  auto i = args.begin();
  Interpreter::atom_value_type result = *i++;
  for (; i != args.end(); ++i) {
    result = result % *i;
  }
  return result;
}
}  // namespace built_in

}  // namespace simpl
