#ifndef PARSER_H_
#define PARSER_H_

#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <variant>
#include <initializer_list>

#include "token.h"

namespace oxid {

class ExprVisitor;

class Expr {
 public:
  class Atom;
  class List;
  virtual void Accept(ExprVisitor* visitor) const = 0;
  virtual ~Expr() {}
};

struct Symbol {
  const std::string name;
};

class Expr::Atom : public Expr {
  using value_type = std::variant<long, double, bool, std::string, Symbol, nullptr_t>;
 public:
  explicit Atom(value_type v): value_(v) {}
  virtual ~Atom() {}
  template <typename T>
  T value() const {
    return std::get<T>(value_);
  }
  virtual void Accept(ExprVisitor* visitor) const override;
 private:
  value_type value_;
};

class Expr::List : public Expr {
 public:
  List(std::initializer_list<std::shared_ptr<Expr>> l): exprs_(l) {}
  explicit List(const std::list<std::shared_ptr<Expr>>& l): exprs_(l) {}
  virtual ~List() {}
  private:
  const std::list<std::shared_ptr<Expr>> exprs_;
};

class ExprVisitor {
 public:
  virtual void Visit(const Expr::List& expr) = 0;
  virtual void Visit(const Expr::Atom& expr) = 0;
};

class Parser {
  using token_list_t = std::list<Token>;
  using expr_ptr = std::shared_ptr<Expr>;
  public:
  explicit Parser(const token_list_t& tokens): tokens_(tokens), current_(tokens_.begin()) {}
  std::shared_ptr<Expr> Parse();
  private:
  expr_ptr ParseExpr();
  expr_ptr ParseList();
  expr_ptr ParseAtom();
  bool Match(Token::Type type);
  bool Check(Token::Type type) const;
  const Token& Advance();
  bool AtEnd() const { return Peek().type == Token::kEof; }
  const Token& Peek() const { return *current_; }
  const Token& Previous() const { return *(std::prev(current_)); }
  const token_list_t tokens_;
  token_list_t::const_iterator current_;
};

}  // namespace oxid

#endif  // PARSER_H_
