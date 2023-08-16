// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/arithmetic.h"

#include <numeric>
#include <string>

#include "simpl/ast.h"
#include "simpl/concepts.hh"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

namespace {

constexpr struct {
  Expr operator()(auto, auto) const {
    throw std::runtime_error("Invalid type for operator +");
  }

  template <typename T1, typename T2>
    requires Numerical<T1> && Numerical<T2>
  Expr operator()(const T1& lhs, const T2& rhs) const {
    return Expr{lhs + rhs};
  }
} Plus;

constexpr struct {
  Expr operator()(auto, auto) const {
    throw std::runtime_error("Invalid type for operator -");
  }

  template <typename T1, typename T2>
    requires Numerical<T1> && Numerical<T2>
  Expr operator()(const T1& lhs, const T2& rhs) const {
    return Expr{lhs - rhs};
  }
} Minus;

constexpr struct {
  Expr operator()(auto, auto) const {
    throw std::runtime_error("Invalid type for operator *");
  }

  template <typename T1, typename T2>
    requires Numerical<T1> && Numerical<T2>
  Expr operator()(const T1& lhs, const T2& rhs) const {
    return Expr{lhs * rhs};
  }
} Times;

constexpr struct {
  Expr operator()(auto, auto) const {
    throw std::runtime_error("Invalid type for operator /");
  }

  template <typename T1, typename T2>
    requires Numerical<T1> && Numerical<T2>
  Expr operator()(const T1& lhs, const T2& rhs) const {
    return Expr{lhs / rhs};
  }
} Devide;

}  // anonymous namespace

static Expr operator+(const Expr& lhs, const Expr& rhs) {
  return std::visit(Plus, lhs, rhs);
}

static Expr operator-(const Expr& lhs, const Expr& rhs) {
  return std::visit(Minus, lhs, rhs);
}

static Expr operator*(const Expr& lhs, const Expr& rhs) {
  return std::visit(Times, lhs, rhs);
}

static Expr operator/(const Expr& lhs, const Expr& rhs) {
  return std::visit(Devide, lhs, rhs);
}

static Expr operator%(const Expr& lhs, const Expr& rhs) {
  if (holds<int_type>(lhs) && holds<int_type>(rhs)) {
    return Expr{std::get<int_type>(lhs) % std::get<int_type>(rhs)};
  }
  throw std::runtime_error(
      "Invalid types for operator %");  // FIXME: error handling
}

Expr Sum::FnCall(Interpreter*, const args_type& args) {
  return std::accumulate(
      args.begin(), args.end(), Expr(0),
      [](const Expr& lhs, const Expr& rhs) { return lhs + rhs; });
}

Expr Substract::FnCall(Interpreter*, const args_type& args) {
  auto i = args.begin();
  const Expr& init = *i++;
  auto result = std::accumulate(
      i, args.end(), init,
      [](const Expr& lhs, const Expr& rhs) { return lhs - rhs; });
  return args.size() > 1 ? result : Expr(0) - init;
}

Expr Multiply::FnCall(Interpreter*, const args_type& args) {
  return std::accumulate(
      args.begin(), args.end(), Expr(1),
      [](const Expr& lhs, const Expr& rhs) { return lhs * rhs; });
}

Expr Divide::FnCall(Interpreter*, const args_type& args) {
  auto i = args.begin();
  const Expr& init = *i++;
  return std::accumulate(
      i, args.end(), init,
      [](const Expr& lhs, const Expr& rhs) { return lhs / rhs; });
}

Expr Modulo::FnCall(Interpreter*, const args_type& args) {
  auto i = args.begin();
  const Expr& init = *i++;
  return std::accumulate(
      i, args.end(), init,
      [](const Expr& lhs, const Expr& rhs) { return lhs % rhs; });
}

}  // namespace built_in

}  // namespace simpl
