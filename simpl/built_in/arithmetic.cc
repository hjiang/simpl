// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/arithmetic.h"

#include <string>

#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

static Expr operator+(const Expr& lhs, const Expr& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) + std::get<int_type>(rhs)};
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) + std::get<float_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) + std::get<int_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) + std::get<float_type>(rhs)};
  }
  throw std::runtime_error(
      "Invalid types for operator +");  // FIXME: error handling
}

static Expr operator-(const Expr& lhs, const Expr& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) - std::get<int_type>(rhs)};
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) - std::get<float_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) - std::get<int_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) - std::get<float_type>(rhs)};
  }
  throw std::runtime_error(
      "Invalid types for operator -");  // FIXME: error handling
}

static Expr operator*(const Expr& lhs, const Expr& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) * std::get<int_type>(rhs)};
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) * std::get<float_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) * std::get<int_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) * std::get<float_type>(rhs)};
  }
  throw std::runtime_error(
      "Invalid types for operator *");  // FIXME: error handling
}

static Expr operator/(const Expr& lhs, const Expr& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) / std::get<int_type>(rhs)};
  }
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) / std::get<float_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) / std::get<int_type>(rhs)};
  }
  if (holds<float_type>(lhs) && holds<float_type>(rhs)) {
    return Expr{std::get<float_type>(lhs) / std::get<float_type>(rhs)};
  }
  throw std::runtime_error(
      "Invalid types for operator /");  // FIXME: error handling
}

static Expr operator%(const Expr& lhs, const Expr& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) % std::get<int_type>(rhs)};
  }
  throw std::runtime_error(
      "Invalid types for operator %");  // FIXME: error handling
}

Expr Sum::FnCall(Interpreter*, const args_type& args) {
  Expr result = args.front();
  auto i = args.begin();
  ++i;
  for (; i != args.end(); ++i) {
    result = result + *i;
  }
  return result;
}

Expr Substract::FnCall(Interpreter*, const args_type& args) {
  auto i = args.begin();
  Expr result = *i++;
  for (; i != args.end(); ++i) {
    result = result - *i;
  }
  return args.size() > 1 ? result : Expr(0) - result;
}

Expr Multiply::FnCall(Interpreter*, const args_type& args) {
  Expr result{1};
  for (auto arg : args) {
    result = result * arg;
  }
  return result;
}
Expr Divide::FnCall(Interpreter*, const args_type& args) {
  auto i = args.begin();
  Expr result = *i++;
  for (; i != args.end(); ++i) {
    result = result / *i;
  }
  return result;
}
Expr Modulo::FnCall(Interpreter*, const args_type& args) {
  auto i = args.begin();
  Expr result = *i++;
  for (; i != args.end(); ++i) {
    result = result % *i;
  }
  return result;
}
}  // namespace built_in

}  // namespace simpl
