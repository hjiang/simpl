#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include <string>
#include <variant>
#include <initializer_list>

namespace oxid {
class Expr {
 public:
  class List;
  class Atom;

  virtual ~Expr() {}
};

struct Symbol {
  const std::string name;
};

class Expr::Atom : public Expr {
  using value_type = std::variant<int, double, bool, std::string, Symbol>;
 public:
  explicit Atom(value_type v): value_(v) {}
  virtual ~Atom() {}
 private:
  value_type value_;
};

class Expr::List : public Expr {
 public:
  List(std::initializer_list<Expr> l): exprs_(l) {}
  virtual ~List() {}
  private:
  const std::list<Expr> exprs_;
};

}  // namespace oxid

#endif  // PARSER_H_
