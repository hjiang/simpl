// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_AST_H_
#define SIMPL_AST_H_

#include <list>
#include <memory>
#include <string>
#include <utility>

#include "simpl/config.h"

namespace simpl {

class Expr {
 public:
  class And;
  class Atom;
  class Def;
  class Do;
  class Fn;
  class Let;
  class List;
  class Quoted;
  class Visitor;

  struct Symbol;

  virtual void Accept(Visitor* visitor) const = 0;
  virtual ~Expr() {}
};

struct Expr::Symbol {
  std::string name;
};

class Callable;

using expr_ptr_t = std::shared_ptr<const Expr>;
using expr_list_t = std::list<expr_ptr_t>;
using callable_ptr_t = std::shared_ptr<Callable>;

class Expr::List : public Expr {
 public:
  explicit List(expr_list_t&& l) : exprs_(std::move(l)) {}
  virtual ~List() {}
  void Accept(Expr::Visitor* visitor) const override;
  const expr_list_t& exprs() const { return exprs_; }

 private:
  const expr_list_t exprs_;
};

class Expr::Atom : public Expr {
 public:
  using value_type =
      std::variant<int_type, float_type, bool, std::string, Symbol,
                   std::nullptr_t, callable_ptr_t, std::shared_ptr<List>>;

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
  void Accept(Expr::Visitor* visitor) const override;

 private:
  value_type value_;
};

class Expr::Quoted : public Expr {
 public:
  explicit Quoted(expr_ptr_t expr) : expr_(expr) {}
  virtual ~Quoted() = default;
  void Accept(Expr::Visitor* visitor) const override;
  const Expr& expr() const { return *expr_; }

 private:
  const expr_ptr_t expr_;
};

class Expr::Do : public Expr {
 public:
  explicit Do(expr_list_t&& exprs) : exprs_(std::move(exprs)) {}
  virtual ~Do() = default;
  void Accept(Expr::Visitor* visitor) const override;
  const expr_list_t& exprs() const { return exprs_; }

 private:
  const expr_list_t exprs_;
};

class Expr::Def : public Expr {
 public:
  Def(std::string name, expr_ptr_t expr)
      : name_(std::move(name)), expr_(std::move(expr)) {}
  virtual ~Def() {}
  void Accept(Expr::Visitor* visitor) const override;
  const std::string& name() const { return name_; }
  const Expr& expr() const { return *expr_; }

 private:
  const std::string name_;
  const expr_ptr_t expr_;
};

class Expr::Let : public Expr {
 public:
  using binding_t = std::pair<std::string, expr_ptr_t>;
  using binding_list_t = std::list<binding_t>;
  Let(binding_list_t&& bindings, expr_list_t&& body)
      : bindings_(std::move(bindings)), body_(std::move(body)) {}
  virtual ~Let() = default;
  void Accept(Expr::Visitor* visitor) const override;
  const binding_list_t& bindings() const { return bindings_; }
  const expr_list_t& body() const { return body_; }

 private:
  const binding_list_t bindings_;
  const expr_list_t body_;
};

class Expr::Fn : public Expr {
 public:
  using param_list_t = std::list<std::string>;
  using body_t = expr_list_t;
  Fn(param_list_t&& params, body_t&& body)
      : params_(std::move(params)), body_(std::move(body)) {}
  virtual ~Fn() = default;
  void Accept(Expr::Visitor* visitor) const override;
  const param_list_t& params() const { return params_; }
  const body_t& body() const { return body_; }

 private:
  const param_list_t params_;
  const body_t body_;
};

class Expr::And : public Expr {
 public:
  explicit And(expr_list_t&& terms) : terms_(std::move(terms)) {}
  virtual ~And() = default;
  void Accept(Expr::Visitor* visitor) const override;
  const expr_list_t& terms() const { return terms_; }

 private:
  const expr_list_t terms_;
};

class Expr::Visitor {
 public:
  virtual void Visit(const Expr::And& expr) = 0;
  virtual void Visit(const Expr::Atom& expr) = 0;
  virtual void Visit(const Expr::Def& expr) = 0;
  virtual void Visit(const Expr::Do& expr) = 0;
  virtual void Visit(const Expr::Fn& expr) = 0;
  virtual void Visit(const Expr::Let& expr) = 0;
  virtual void Visit(const Expr::List& expr) = 0;
  virtual void Visit(const Expr::Quoted& expr) = 0;
  virtual ~Visitor() {}
};

}  // namespace simpl

#endif  // SIMPL_AST_H_
