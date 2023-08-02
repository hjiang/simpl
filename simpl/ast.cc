// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/ast.h"

#include <iterator>
#include <ostream>
#include <ranges>

namespace simpl {

namespace rv = std::views;

Quoted::Quoted(const Expr& expr) : expr_(std::make_unique<Expr>(expr)) {}

Quoted::Quoted(const Quoted& other)
    : expr_(std::make_unique<Expr>(*other.expr_)) {}

Quoted::Quoted(Quoted&& other) noexcept : expr_(std::move(other.expr_)) {}

Quoted& Quoted::operator=(const Quoted& other) {
  *expr_ = *(other.expr_);
  return *this;
}

Quoted& Quoted::operator=(Quoted&& other) noexcept {
  expr_ = std::move(other.expr_);
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Symbol& s) {
  return os << s.name;
}

std::ostream& operator<<(std::ostream& os, const List& l) {
  os << '(';
  if (!l.empty()) {
    for (auto& e : rv::take(l, l.size() - 1)) {
      os << e;
      os << ' ';
    }
    os << l.back();
  }
  return os << ')';
}

std::ostream& operator<<(std::ostream& os, const Vector& vec) {
  os << '[';
  if (!vec.empty()) {
    for (auto& e : rv::take(vec, vec.size() - 1)) {
      os << e;
      os << ' ';
    }
    os << vec.back();
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

}  // namespace simpl
