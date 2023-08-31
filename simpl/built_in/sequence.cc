// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/sequence.h"

#include <concepts>
#include <memory>
#include <ranges>
#include <stdexcept>
#include <utility>
#include <variant>

#include "simpl/built_in/util.h"
#include "simpl/overload.h"

namespace simpl {
namespace built_in {

namespace sv = std::views;

namespace {

template <typename T>
concept HasEmpty = requires(T t) {
                     { t.empty() } -> std::convertible_to<bool>;
                   };  // NOLINT

template <typename T>
concept HasFront = requires(T t) {
                     {
                       t.front()
                       } -> std::convertible_to<typename T::value_type>;
                   };  // NOLINT

}  // anonymous namespace

Expr Cons::FnCall(Interpreter*, args_type&& args) {
  CheckArity("cons", args, 2);
  return std::visit(
      Overload{
          [](auto&&, auto&&) -> Expr {
            throw std::runtime_error("cons: invalid argument types");
          },
          [](auto&& car, List&& cdr) {
            cdr.push_front(std::forward<decltype(car)>(car));
            return Expr{std::move(cdr)};
          },
          [](auto&& car, std::nullptr_t) {
            return Expr{List{std::forward<decltype(car)>(car)}};
          },
      },
      std::move(args.front()), std::move(args.back()));
}

Expr Head::FnCall(Interpreter*, args_type&& args) {
  CheckArity("head", args, 1);
  return std::visit(
      Overload{
          [](auto&&) -> Expr {
            throw std::runtime_error("head: invalid argument type");
          },
          [](HasFront auto&& seq) { return Expr{std::move(seq.front())}; },
      },
      std::move(args.front()));
}

struct TailVisitor {
  template <typename T>
  Expr operator()(T&& seq) const {
    if constexpr (std::same_as<T, List> || std::same_as<T, Vector>) {
      auto tail = sv::drop(seq, 1);
      return T(make_move_iterator(tail.begin()),
               make_move_iterator(tail.end()));
    } else {
      throw std::runtime_error("head: invalid argument type");
    }
  }
};

Expr Tail::FnCall(Interpreter*, args_type&& args) {
  CheckArity("tail", args, 1);
  return std::visit(TailVisitor{}, std::move(args.front()));
}

Expr Empty::FnCall(Interpreter*, args_type&& args) {
  CheckArity("empty?", args, 1);
  return Expr{std::visit(Overload{[](HasEmpty auto&& c) { return c.empty(); },
                                  [](auto&&) -> bool {
                                    throw std::runtime_error(
                                        "empty?: invalid argument type");
                                  }},
                         std::move(args.front()))};
}

}  // namespace built_in
}  // namespace simpl
