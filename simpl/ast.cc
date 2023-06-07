// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/ast.h"

#include <iterator>
#include <ostream>

namespace simpl {

std::ostream& operator<<(std::ostream& os, const Symbol& s) {
  return os << s.name;
}

std::ostream& operator<<(std::ostream& os, const List& l) {
  os << '(';
  for (auto& e : l.exprs()) {
    os << *e;
    if (e != l.exprs().back()) {
      os << ' ';
    }
  }
  return os << ')';
}

std::ostream& operator<<(std::ostream& os, const Vector& vec) {
  os << '[';
  for (auto& e : vec.exprs()) {
    os << *e;
    if (e != vec.exprs().back()) {
      os << ' ';
    }
  }
  return os << ']';
}

std::ostream& operator<<(std::ostream& os, const Quoted& qt) {
  return os << '\'' << qt.expr();
}

// Defined in callable.cc
std::ostream& operator<<(std::ostream& os, const Callable&);

struct ExprPrinter {
  std::ostream& os;
  void operator()(const Symbol& s) { os << s; }
  void operator()(nullptr_t) { os << "nil"; }
  void operator()(const std::string& s) { os << "\"" << s << "\""; }
  void operator()(bool b) { os << (b ? "true" : "false"); }
  template <typename T>
  void operator()(std::shared_ptr<T> ep) {
    os << *ep;
  }
  template <typename T>
  void operator()(T b) {
    os << b;
  }
};

std::ostream& operator<<(std::ostream& os, const Expr& e) {
  ExprPrinter printer{os};
  std::visit(printer, e);
  return os;
}

Expr List::Cons(const Expr& expr) const {
  expr_list_t exprs(exprs_);
  exprs.push_front(make_shared<Expr>(expr));
  auto lst = std::make_unique<List>(exprs);
  return lst;
}

}  // namespace simpl
