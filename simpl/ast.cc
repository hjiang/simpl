// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/ast.hh"

#include <memory>
#include <ostream>
#include <ranges>
#include <string>
#include <utility>

#include "simpl/overload.hh"

namespace simpl {

namespace rv = std::views;

Quoted::Quoted(const Expr& expr, Kind kind)
    : expr_(std::make_unique<Expr>(expr)), kind_(kind) {}

Quoted::Quoted(const Quoted& other)
    : expr_(std::make_unique<Expr>(*other.expr_)), kind_(other.kind_) {}

Quoted::Quoted(Quoted&& other) noexcept
    : expr_(std::move(other.expr_)), kind_(other.kind_) {}

Quoted& Quoted::operator=(const Quoted& other) {
  *expr_ = *(other.expr_);
  kind_ = other.kind_;
  return *this;
}

Quoted& Quoted::operator=(Quoted&& other) noexcept {
  expr_ = std::move(other.expr_);
  kind_ = other.kind_;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const Symbol& s) {
  return os << s.name;
}

std::ostream& operator<<(std::ostream& os, const List& l) {
  os << '(';
  if (!l.empty()) {
    for (const auto& e : rv::take(l, l.size() - 1)) {
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
    for (const auto& e : rv::take(vec, vec.size() - 1)) {
      os << e;
      os << ' ';
    }
    os << vec.back();
  }
  return os << ']';
}

std::ostream& operator<<(std::ostream& os, const Quoted& qt) {
  switch (qt.kind()) {
    case Quoted::Kind::kQuote:
      return os << '\'' << qt.expr();
    case Quoted::Kind::kSyntaxQuote:
      return os << '`' << qt.expr();
    case Quoted::Kind::kUnquote:
      return os << '~' << qt.expr();
    case Quoted::Kind::kSplice:
      return os << "~@" << qt.expr();
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const TailCall&) {
  return os << "<tail-call>";
}

// Defined in callable.cc
std::ostream& operator<<(std::ostream& os, const Callable&);

struct ExprPrinter {
  std::ostream& os;
  void operator()(const Symbol& s) { os << s; }
  void operator()(std::nullptr_t) { os << "nil"; }
  void operator()(const std::string& s) { os << "\"" << s << "\""; }
  void operator()(bool b) { os << (b ? "true" : "false"); }
  void operator()(const TailCall& tc) { os << tc; }
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

std::size_t Hash::operator()(const Expr& expr) const {
  static Hash hash;
  return std::visit(
      Overload{[](int_type i) { return std::hash<int_type>{}(i); },
               [](float_type f) { return std::hash<float_type>{}(f); },
               [](bool b) { return std::hash<bool>{}(b); },
               [](const std::string& s) { return std::hash<std::string>{}(s); },
               [](const Symbol& s) { return s.hash(); },
               [](const Keyword& kw) { return kw.hash(); },
               [](std::nullptr_t) { return std::hash<uint32_t>{}(0xdeadbeef); },
               [](callable_ptr_t c) { return std::hash<callable_ptr_t>{}(c); },
               [](const List& l) {
                 std::size_t seed = 0;
                 for (auto& e : l) {
                   seed ^= hash(e) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                 }
                 return seed;
               },
               [](const Vector& v) {
                 std::size_t seed = 0;
                 for (auto& e : v) {
                   seed ^= hash(e) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                 }
                 return seed;
               },
               [](const Quoted& qt) { return qt.hash(); },
               [](const Map& m) {
                 std::size_t seed = 0;
                 for (auto& [k, v] : m) {
                   seed ^= hash(k) + hash(v) * 0x9e3779b9;
                 }
                 return seed;
               },
               [](const TailCall&) -> std::size_t {
                 throw std::logic_error("TailCall cannot be hashed");
               }},
      expr);
}

bool Quoted::operator==(const Quoted& other) const {
  return kind_ == other.kind_ && *expr_ == *other.expr_;
}

}  // namespace simpl
