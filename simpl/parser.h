// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_PARSER_H_
#define SIMPL_PARSER_H_

#include <initializer_list>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <variant>

#include "simpl/token.h"

namespace simpl {

class Expr {
 public:
  class And;
  class Atom;
  class Def;
  class Do;
  class Fn;
  class If;
  class Let;
  class List;
  class Or;
  class Visitor;

  virtual void Accept(Visitor* visitor) const = 0;
  virtual ~Expr() {}
  struct Symbol {
    const std::string name;
  };
};

using token_list_t = std::list<Token>;
using expr_ptr_t = std::unique_ptr<const Expr>;
using expr_list_t = std::list<expr_ptr_t>;

class Expr::Atom : public Expr {
  using value_type = std::variant<int_type, float_type, bool, std::string,
                                  Symbol, std::nullptr_t>;

 public:
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

class Expr::List : public Expr {
 public:
  explicit List(expr_list_t&& l) : exprs_(std::move(l)) {}
  virtual ~List() {}
  void Accept(Expr::Visitor* visitor) const override;
  const expr_list_t& exprs() const { return exprs_; }

 private:
  const expr_list_t exprs_;
};

class Expr::Do: public Expr {
 public:
  explicit Do(expr_list_t&& exprs)
      : exprs_(std::move(exprs)) {}
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

class Expr::If : public Expr {
 public:
  If(expr_ptr_t&& cond, expr_ptr_t&& then,
     expr_ptr_t&& otherwise)
      : cond_(std::move(cond)),
        then_(std::move(then)),
        otherwise_(std::move(otherwise)) {}
  virtual ~If() = default;
  void Accept(Expr::Visitor* visitor) const override;
  const Expr& cond() const { return *cond_; }
  const Expr& then() const { return *then_; }
  const Expr& otherwise() const { return *otherwise_; }

 private:
  const expr_ptr_t cond_;
  const expr_ptr_t then_;
  const expr_ptr_t otherwise_;
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

class Expr::Or: public Expr {
 public:
  using term_list_t = expr_list_t;
  explicit Or(term_list_t&& terms) : terms_(std::move(terms)) {}
  virtual ~Or() = default;
  void Accept(Expr::Visitor* visitor) const override;
  const term_list_t& terms() const { return terms_; }
 private:
  const term_list_t terms_;
};

class Expr::And: public Expr {
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
  virtual void Visit(const Expr::If& expr) = 0;
  virtual void Visit(const Expr::Let& expr) = 0;
  virtual void Visit(const Expr::List& expr) = 0;
  virtual void Visit(const Expr::Or& expr) = 0;
  virtual ~Visitor() {}
};

class Parser {
 public:
  explicit Parser(const token_list_t& tokens)
      : tokens_(tokens), current_(tokens_.begin()) {}
  expr_list_t Parse();

 private:
  class ParseError : std::runtime_error {
   public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
  };
  ParseError Error(const Token& token, const std::string& msg);
  expr_ptr_t ParseAnd();
  expr_ptr_t ParseAtom();
  expr_ptr_t ParseDef();
  expr_ptr_t ParseDefn();
  expr_ptr_t ParseDo();
  expr_ptr_t ParseExpr();
  expr_ptr_t ParseFn();
  expr_ptr_t ParseIf();
  expr_ptr_t ParseLet();
  expr_ptr_t ParseList();
  expr_ptr_t ParseOr();
  Expr::Fn::param_list_t ParseParamList();
  Expr::Let::binding_list_t ParseBindings();
  Expr::Let::binding_t ParseBinding();
  expr_list_t ParseExprs();
  bool Match(Token::Type type);
  bool Check(Token::Type type) const;
  const Token& Consume(Token::Type type, const std::string& msg);
  const Token& Advance();
  bool AtEnd() const { return Peek().type == Token::kEof; }
  const Token& Peek() const { return *current_; }
  const Token& Previous() const { return *(std::prev(current_)); }
  const token_list_t tokens_;
  token_list_t::const_iterator current_;
};

}  // namespace simpl

#endif  // SIMPL_PARSER_H_

// Local Variables:
// compile-command : "bazel test //simpl:all"
// End:
