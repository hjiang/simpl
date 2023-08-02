// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/sequence.h"

#include <concepts>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <variant>

#include "simpl/built_in/util.h"

namespace simpl {
namespace built_in {

namespace sv = std::views;

struct ConsVisitor {
  template <typename T, typename U>
  Expr operator()(T&&, const U&) const {
    throw std::runtime_error("cons: invalid argument type");
  }

  template <typename T>
  Expr operator()(T&& car, const List& cdr) const {
    List l(cdr);
    l.push_front(std::forward<T>(car));
    return l;
  }

  template <typename T>
  Expr operator()(T&& car, nullptr_t) const {
    return List{std::forward<T>(car)};
  }
};

Expr Cons::FnCall(Interpreter*, const args_type& args) {
  CheckArity("cons", args, 2);
  return std::visit(ConsVisitor(), args.front(), args.back());
}

struct HeadVisitor {
  template <typename T>
  Expr operator()(const T&) const {
    throw std::runtime_error("head: invalid argument type");
  }
};

template <>
Expr HeadVisitor::operator()(const List& list) const {
  return list.front();
}

template <>
Expr HeadVisitor::operator()(const Vector& vec) const {
  return vec.front();
}

Expr Head::FnCall(Interpreter*, const args_type& args) {
  CheckArity("head", args, 1);
  return std::visit(HeadVisitor(), args.front());
}

struct TailVisitor {
  template <typename T>
  Expr operator()(const T& seq) const {
    if constexpr (std::same_as<T, List> || std::same_as<T, Vector>) {
      auto tail = sv::drop(seq, 1);
      return T(tail.begin(), tail.end());
    } else {
      throw std::runtime_error("head: invalid argument type");
    }
  }
};

Expr Tail::FnCall(Interpreter*, const args_type& args) {
  CheckArity("tail", args, 1);
  return std::visit(TailVisitor(), args.front());
}

}  // namespace built_in
}  // namespace simpl
