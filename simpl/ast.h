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
class ExprVisitor;
class Callable;
using callable_ptr_t = std::shared_ptr<Callable>;
using list_ptr_t = std::shared_ptr<List>;
using vector_ptr_t = std::shared_ptr<Vector>;
using quoted_ptr_t = std::shared_ptr<Quoted>;

using Expr = std::variant<int_type, float_type, bool, std::string, Symbol,
                          std::nullptr_t, callable_ptr_t, list_ptr_t,
                          vector_ptr_t, quoted_ptr_t>;

std::ostream& operator<<(std::ostream& os, const Expr& e);

struct Symbol {
  std::string name;
};

std::ostream& operator<<(std::ostream& os, const Symbol& s);

using expr_ptr_t = std::shared_ptr<const Expr>;
using expr_list_t = std::list<expr_ptr_t>;

class List {
 public:
  template <typename T>
  explicit List(T&& l) : exprs_(std::forward<T>(l)) {}
  virtual ~List() {}
  Expr Cons(const Expr& expr) const;
  const Expr& Head() const;
  Expr Tail() const;
  const expr_list_t& exprs() const { return exprs_; }

 private:
  const expr_list_t exprs_;
};

std::ostream& operator<<(std::ostream& os, const List& l);

class Vector {
 public:
  using vector_impl_t = std::vector<expr_ptr_t>;
  explicit Vector(vector_impl_t&& l) : exprs_(std::move(l)) {}
  virtual ~Vector() = default;
  const vector_impl_t& exprs() const { return exprs_; }

 private:
  const std::vector<expr_ptr_t> exprs_;
};

std::ostream& operator<<(std::ostream& os, const Vector& vec);

class Quoted {
 public:
  explicit Quoted(expr_ptr_t expr) : expr_(expr) {}
  virtual ~Quoted() = default;
  const Expr& expr() const { return *expr_; }

 private:
  const expr_ptr_t expr_;
};

std::ostream& operator<<(std::ostream& os, const Quoted& qt);

}  // namespace simpl

#endif  // SIMPL_AST_H_
