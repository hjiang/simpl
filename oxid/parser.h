#ifndef PARSER_H_
#define PARSER_H_

#include <initializer_list>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <variant>

#include "token.h"

namespace oxid {

class Expr {
 public:
  class Atom;
  class List;
  class Visitor;
  virtual void Accept(Visitor* visitor) const = 0;
  virtual ~Expr() {}
  struct Symbol {
    const std::string name;
  };
};

class Expr::Atom : public Expr {
  using value_type =
      std::variant<long, double, bool, std::string, Symbol, nullptr_t>;

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
  virtual void Accept(Expr::Visitor* visitor) const override;

 private:
  value_type value_;
};

class Expr::List : public Expr {
 public:
  explicit List(std::list<std::unique_ptr<Expr>>&& l) : exprs_(std::move(l)) {}
  virtual ~List() {}
  virtual void Accept(Expr::Visitor* visitor) const override;
  const std::list<std::unique_ptr<Expr>>& exprs() const { return exprs_; }

 private:
  const std::list<std::unique_ptr<Expr>> exprs_;
};

class Parser {
  using token_list_t = std::list<Token>;
  using expr_ptr = std::unique_ptr<Expr>;

 public:
  explicit Parser(const token_list_t& tokens)
      : tokens_(tokens), current_(tokens_.begin()) {}
  std::unique_ptr<Expr> Parse();

 private:
  class ParseError : std::runtime_error {
   public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
  };
  ParseError Error(const Token& token, const std::string& msg);
  expr_ptr ParseExpr();
  expr_ptr ParseList();
  expr_ptr ParseAtom();
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

class Expr::Visitor {
 public:
  virtual void Visit(const Expr::List& expr) = 0;
  virtual void Visit(const Expr::Atom& expr) = 0;
  virtual ~Visitor() {}
};

}  // namespace oxid

#endif  // PARSER_H_

// Local Variables:
// compile-command : "bazel test //oxid:all"
// End:
