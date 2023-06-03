// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_AST_H_
#define SIMPL_AST_H_

#include <list>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "simpl/config.h"

namespace simpl {

class Atom;
class List;
class Vector;
class Quoted;
struct Symbol;
class ExprVisitor;
class Expr {
 public:
  virtual void Accept(ExprVisitor* visitor) const = 0;
  virtual ~Expr() {}
};

struct Symbol {
  std::string name;
};

class Callable;

using expr_ptr_t = std::shared_ptr<const Expr>;
using expr_list_t = std::list<expr_ptr_t>;
using callable_ptr_t = std::shared_ptr<Callable>;

class List : public Expr {
 public:
  explicit List(expr_list_t&& l) : exprs_(std::move(l)) {}
  virtual ~List() {}
  void Accept(ExprVisitor* visitor) const override;
  const expr_list_t& exprs() const { return exprs_; }

 private:
  const expr_list_t exprs_;
};

class Vector : public Expr {
 public:
  using vector_impl_t = std::vector<expr_ptr_t>;
  explicit Vector(vector_impl_t&& l) : exprs_(std::move(l)) {}
  virtual ~Vector() = default;
  void Accept(ExprVisitor* visitor) const override;
  const vector_impl_t& exprs() const { return exprs_; }

 private:
  const std::vector<expr_ptr_t> exprs_;
};

class Atom : public Expr {
 public:
  using value_type =
      std::variant<int_type, float_type, bool, std::string, Symbol,
                   std::nullptr_t, callable_ptr_t, std::shared_ptr<List>,
                   std::shared_ptr<Vector>>;

  explicit Atom(value_type v) : value_(v) {}
  virtual ~Atom() {}
  template <typename T>
  T value() const {
    return std::get<T>(value_);
  }
  template <typename T>
  bool has_value() const {
    return std::holds_alternative<T>(value_);
  }
  void Accept(ExprVisitor* visitor) const override;

 private:
  value_type value_;
};

class Quoted : public Expr {
 public:
  explicit Quoted(expr_ptr_t expr) : expr_(expr) {}
  virtual ~Quoted() = default;
  void Accept(ExprVisitor* visitor) const override;
  const Expr& expr() const { return *expr_; }

 private:
  const expr_ptr_t expr_;
};

class ExprVisitor {
 public:
  virtual void Visit(const Atom& expr) = 0;
  virtual void Visit(const List& expr) = 0;
  virtual void Visit(const Quoted& expr) = 0;
  virtual void Visit(const Vector& expr) = 0;
  virtual ~ExprVisitor() {}
};

}  // namespace simpl

#endif  // SIMPL_AST_H_
