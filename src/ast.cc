// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/ast.h"

#include <iterator>
#include <ostream>
#include <ranges>

namespace simpl {

namespace rv = std::views;

Quoted::Quoted(const Expr& expr) : expr_(std::make_unique<Expr>(expr)) {}

List::List(std::list<Expr>&& l) : exprs_(std::move(l)) {}

List::List(const std::list<Expr>& l) : exprs_(l) {}

std::ostream& operator<<(std::ostream& os, const Symbol& s) {
  return os << s.name;
}

std::ostream& operator<<(std::ostream& os, const List& l) {
  os << '(';
  for (auto& e : rv::take(l.exprs(), l.exprs().size() - 1)) {
    os << e;
    os << ' ';
  }
  return os << l.exprs().back() << ')';
}

std::ostream& operator<<(std::ostream& os, const Vector& vec) {
  os << '[';
  for (auto& e : rv::take(vec.exprs(), vec.exprs().size() - 1)) {
    os << e;
    os << ' ';
  }
  return os << vec.exprs().back() << ']';
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
  std::list<Expr> exprs(exprs_);
  exprs.push_front(expr);
  return Expr{List(exprs)};
}

const Expr& List::Head() const { return exprs_.front(); }

Expr List::Tail() const {
  std::list<Expr> exprs(exprs_);
  exprs.pop_front();
  return Expr{List(exprs)};
}

Vector::Vector(vector_impl_t&& l) : exprs_(std::move(l)) {}

const Expr& Vector::Head() const { return exprs_.front(); }

Expr Vector::Tail() const {
  auto begin = exprs_.begin();
  std::list<Expr> exprs(++begin, exprs_.end());
  return Expr{List(exprs)};
}

const Expr& Vector::Get(int_type idx) const {
  return exprs_[static_cast<size_t>(idx)];
}

}  // namespace simpl
