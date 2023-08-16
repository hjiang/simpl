// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_AST_H_
#define SIMPL_AST_H_

#include <concepts>
#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "simpl/config.h"

namespace simpl {

class Callable;
using callable_ptr_t = std::shared_ptr<Callable>;
class Expr;

class Hash {
 public:
  std::size_t operator()(const Expr& expr) const;
};

class ExprList : public std::list<Expr> {
  using std::list<Expr>::list;
};

struct Symbol {
  std::string name;
  std::size_t hash() const {
    return std::hash<std::string>{}(name) ^ std::hash<uint32_t>{}(0xdeadbeef);
  }
  bool operator==(const Symbol& other) const { return name == other.name; }
};

class Quoted {
 public:
  Quoted(const Quoted& other);
  Quoted(Quoted&& other) noexcept;
  explicit Quoted(const Expr& expr);
  const Expr& expr() const { return *expr_; }
  Quoted& operator=(const Quoted& other);
  Quoted& operator=(Quoted&& other) noexcept;
  std::size_t hash() const {
    return Hash{}(*expr_) ^ std::hash<uint32_t>{}(0xdeadbeef);
  }
  bool operator==(const Quoted& other) const;

 private:
  std::unique_ptr<Expr> expr_;  // TODO(hjiang): change to unique_ptr
};

// List and ExprList should not be automatically convertible
class List : public std::list<Expr> {
  using std::list<Expr>::list;
};

using Vector = std::vector<Expr>;

using Map = std::unordered_map<Expr, Expr, Hash>;

using ExprBase =
    std::variant<int_type, float_type, bool, std::string, Symbol, nullptr_t,
                 callable_ptr_t, List, Vector, Quoted, Map>;

class Expr : public ExprBase {
 public:
  Expr() = default;
  template <typename T>
  // TODO(hjiang): disable implicit conversion
  // cppcheck-suppress noExplicitConstructor
  Expr(T&& t) : ExprBase(std::forward<T>(t)) {}

  // TODO(hjiang): Do I need to define assignment to enable move?
};

std::ostream& operator<<(std::ostream& os, const Expr& e);
std::ostream& operator<<(std::ostream& os, const List& l);
std::ostream& operator<<(std::ostream& os, const Symbol& s);
std::ostream& operator<<(std::ostream& os, const Vector& vec);
std::ostream& operator<<(std::ostream& os, const Quoted& qt);

}  // namespace simpl

#endif  // SIMPL_AST_H_
