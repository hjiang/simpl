// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/comparison.hh"

#include <compare>
#include <list>
#include <string>
#include <utility>
#include <variant>

#include "simpl/ast.hh"
#include "simpl/interpreter.hh"
#include "simpl/interpreter_util.hh"
#include "simpl/util.hh"

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

constexpr struct {
  int operator()(auto&&, auto&&) const {
    throw std::runtime_error("Incomparable types.");
  }

  template <typename T1, typename T2>
  requires Numeric<T1> && Numeric<T2> && std::three_way_comparable_with<T1, T2>
  int operator()(T1&& lhs, T2&& rhs) const {
    return OrderingToInt(lhs <=> rhs);
  }

  int operator()(std::string&& lhs, std::string&& rhs) const {
    return lhs.compare(std::move(rhs));
  }
} compare;

int Compare(std::list<Expr>&& args) {
  if (args.size() != 2) {
    throw std::runtime_error("comparison requires 2 arguments");
  }
  return std::visit(compare, std::move(args.front()), std::move(args.back()));
}

// Equality check for two Expr values. Returns false for incompatible types.
bool ExprEqual(Expr&& lhs, Expr&& rhs) {
  // Cross-numeric: use variant index to detect mixed numeric types.
  if (lhs == rhs) {
    // std::variant::operator== handles same-type equality correctly for all
    // alternative types that define operator==.
    return true;
  }
  // Cross-numeric comparison: one int, one float.
  if (holds<int_type>(lhs) && holds<float_type>(rhs)) {
    return static_cast<float_type>(std::get<int_type>(lhs)) ==
           std::get<float_type>(rhs);
  }
  if (holds<float_type>(lhs) && holds<int_type>(rhs)) {
    return std::get<float_type>(lhs) ==
           static_cast<float_type>(std::get<int_type>(rhs));
  }
  return false;
}

}  // namespace

Expr Equals::FnCall(Interpreter*, args_type&& args) {
  if (args.size() != 2) {
    throw std::runtime_error("comparison requires 2 arguments");
  }
  return ExprEqual(std::move(args.front()), std::move(args.back()));
}

Expr GreaterThan::FnCall(Interpreter*, args_type&& args) {
  return Compare(std::move(args)) > 0;
}

Expr GreaterThanOrEqualTo::FnCall(Interpreter*, args_type&& args) {
  return Compare(std::move(args)) >= 0;
}

Expr LessThan::FnCall(Interpreter*, args_type&& args) {
  return Compare(std::move(args)) < 0;
}

Expr LessThanOrEqualTo::FnCall(Interpreter*, args_type&& args) {
  return Compare(std::move(args)) <= 0;
}

}  // namespace built_in

}  // namespace simpl
