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

class List;
class Vector;
class Quoted;
struct Symbol;
class Callable;
using callable_ptr_t = std::shared_ptr<Callable>;
class Expr;

using expr_ptr_t = std::shared_ptr<Expr>;
using expr_list_t = std::list<Expr>;

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

class List {
 public:
  explicit List(std::list<Expr>&& l);
  explicit List(const std::list<Expr>& l);
  Expr Cons(const Expr& expr) const;
  const Expr& Head() const;
  Expr Tail() const;
  const std::list<Expr>& exprs() const { return exprs_; }

 private:
  std::list<Expr> exprs_;
};

class Vector {
 public:
  using vector_impl_t = std::vector<Expr>;
  explicit Vector(vector_impl_t&& l);
  const vector_impl_t& exprs() const { return exprs_; }
  const Expr& Head() const;
  Expr Tail() const;
  const Expr& Get(int_type idx) const;

 private:
  vector_impl_t exprs_;
};

using ExprBase = std::variant<int_type, float_type, bool, std::string, Symbol,
                              nullptr_t, callable_ptr_t, List, Vector, Quoted>;

class Expr : public ExprBase {
 public:
  Expr() = default;
  template <typename T>
  // implicit conversion by design
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
