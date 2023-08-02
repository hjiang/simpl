// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_AST_H_
#define SIMPL_AST_H_

#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "simpl/config.h"

namespace simpl {

class Callable;
using callable_ptr_t = std::shared_ptr<Callable>;
class Expr;

class ExprList : public std::list<Expr> {
  using std::list<Expr>::list;
};

struct Symbol {
  std::string name;
};

class Quoted {
 public:
  Quoted(const Quoted& other);
  Quoted(Quoted&& other) noexcept;
  explicit Quoted(const Expr& expr);
  const Expr& expr() const { return *expr_; }
  Quoted& operator=(const Quoted& other);
  Quoted& operator=(Quoted&& other) noexcept;

 private:
  std::unique_ptr<Expr> expr_;  // TODO(hjiang): change to unique_ptr
};

// List and ExprList should not be automatically convertible
class List : public std::list<Expr> {
  using std::list<Expr>::list;
};

using Vector = std::vector<Expr>;

using ExprBase = std::variant<int_type, float_type, bool, std::string, Symbol,
                              nullptr_t, callable_ptr_t, List, Vector, Quoted>;

class Expr : public ExprBase {
 public:
  Expr() = default;
  template <typename T>
  // TODO(hjiang): disable implicit conversion
  Expr(T&& t) : ExprBase(std::forward<T>(t)) {}  // NOLINT

  // TODO(hjiang): Do I need to define assignment to enable move?
};

std::ostream& operator<<(std::ostream& os, const Expr& e);
std::ostream& operator<<(std::ostream& os, const List& l);
std::ostream& operator<<(std::ostream& os, const Symbol& s);
std::ostream& operator<<(std::ostream& os, const Vector& vec);
std::ostream& operator<<(std::ostream& os, const Quoted& qt);

}  // namespace simpl

#endif  // SIMPL_AST_H_
