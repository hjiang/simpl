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

int Compare(const Expr& lhs, const Expr& rhs);

int CompareList(const List& lhs, const List& rhs) {
  auto lhs_it = lhs.begin();
  auto rhs_it = rhs.begin();
  while (lhs_it != lhs.end() && rhs_it != rhs.end()) {
    auto cmp = Compare(*lhs_it, *rhs_it);
    if (cmp != 0) {
      return cmp;
    }
    ++lhs_it;
    ++rhs_it;
  }
  if (lhs_it == lhs.end() && rhs_it == rhs.end()) {
    return 0;
  }
  if (lhs_it == lhs.end()) {
    return -1;
  }
  return 1;
}

int Compare(const Expr& lhs, const Expr& rhs) {
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
  if (holds<List>(lhs) && holds<List>(rhs)) {
    return CompareList(std::get<List>(lhs), std::get<List>(rhs));
  }
  throw std::runtime_error("Incomparable types");  // FIXME: error handling
}

int Compare(const std::list<Expr>& args) {
  if (args.size() != 2) {
    throw std::runtime_error("comparison requires 2 arguments");
  }
  auto lhs = args.front();
  auto rhs = args.back();
  return Compare(lhs, rhs);
}

}  // namespace

Expr Equals::FnCall(Interpreter*, const args_type& args) {
  return Compare(args) == 0;
}

Expr GreaterThan::FnCall(Interpreter*, const args_type& args) {
  return Compare(args) > 0;
}

Expr GreaterThanOrEqualTo::FnCall(Interpreter*, const args_type& args) {
  return Compare(args) >= 0;
}

Expr LessThan::FnCall(Interpreter*, const args_type& args) {
  return Compare(args) < 0;
}

Expr LessThanOrEqualTo::FnCall(Interpreter*, const args_type& args) {
  return Compare(args) <= 0;
}

}  // namespace built_in

}  // namespace simpl
