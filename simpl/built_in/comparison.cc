// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/comparison.h"

#include <compare>
#include <list>
#include <string>
#include <variant>

#include "simpl/ast.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"
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

namespace {

constexpr struct {
  int operator()(auto, auto) const {
    throw std::runtime_error("Incomparable types.");
  }

  template <typename T1, typename T2>
    requires Numeric<T1> && Numeric<T2> &&
             std::three_way_comparable_with<T1, T2>
  int operator()(T1 lhs, T2 rhs) const {
    return OrderingToInt(lhs <=> rhs);
  }

  int operator()(const std::string& lhs, const std::string& rhs) const {
    return lhs.compare(rhs);
  }
} compare;

}  // anonymous namespace

int Compare(const std::list<Expr>& args) {
  if (args.size() != 2) {
    throw std::runtime_error("comparison requires 2 arguments");
  }
  auto lhs = args.front();
  auto rhs = args.back();
  return std::visit(compare, lhs, rhs);
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
