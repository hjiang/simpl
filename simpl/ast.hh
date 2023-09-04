// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_AST_H_
#define SIMPL_AST_H_

#include <concepts>
#include <list>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "simpl/config.hh"

namespace simpl {

class Callable;
using callable_ptr_t = std::shared_ptr<Callable>;
class Expr;

class Hash {
 public:
  std::size_t operator()(const Expr& expr) const;
};

class ExprList : public std::list<Expr> {
  using std::list<Expr>::list;
};

struct Symbol {
  std::string name;
  std::size_t hash() const {
    return std::hash<std::string>{}(name) ^ std::hash<uint32_t>{}(0xdeadbeef);
  }
  bool operator==(const Symbol& other) const = default;
};

struct Keyword {
  std::string name;
  std::size_t hash() const {
    return std::hash<std::string>{}(name) ^ std::hash<uint32_t>{}(0xbeefdead);
  }
  bool operator==(const Keyword& other) const = default;
};

class Quoted {
 public:
  Quoted(const Quoted& other);
  Quoted(Quoted&& other) noexcept;
  explicit Quoted(const Expr& expr);
  const Expr& expr() const { return *expr_; }
  Quoted& operator=(const Quoted& other);
  Quoted& operator=(Quoted&& other) noexcept;
  std::size_t hash() const {
    static Hash hash;
    return hash(*expr_) ^ std::hash<uint32_t>{}(0xdeadbeef);
  }
  bool operator==(const Quoted& other) const;

 private:
  std::unique_ptr<Expr> expr_;
};

// List and ExprList should not be automatically convertible
class List : public std::list<Expr> {
  using std::list<Expr>::list;
};

using Vector = std::vector<Expr>;

using Map = std::unordered_map<Expr, Expr, Hash>;

using ExprBase = std::variant<int_type, float_type, bool, std::string, Symbol,
                              std::nullptr_t, callable_ptr_t, List, Vector,
                              Quoted, Map, Keyword>;

class Expr : public ExprBase {
 public:
  using ExprBase::ExprBase;
};

std::ostream& operator<<(std::ostream& os, const Expr& e);
std::ostream& operator<<(std::ostream& os, const List& l);
std::ostream& operator<<(std::ostream& os, const Symbol& s);
std::ostream& operator<<(std::ostream& os, const Vector& vec);
std::ostream& operator<<(std::ostream& os, const Quoted& qt);

}  // namespace simpl

#endif  // SIMPL_AST_H_
