// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_INTERPRETER_H_
#define SIMPL_INTERPRETER_H_

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "simpl/ast.h"

namespace simpl {

class Callable;

class Interpreter {
 public:
  class Environment {
   public:
    explicit Environment(std::shared_ptr<Environment> parent = nullptr)
        : values_(), parent_(parent) {}

    void Define(const std::string& name, auto&& value) {
      if (parent_) {
        return parent_->Define(name, std::forward<decltype(value)>(value));
      } else {
        values_[name] = std::forward<decltype(value)>(value);
      }
    }

    void Bind(const std::string& name, auto&& value) {
      values_.insert_or_assign(name, std::forward<decltype(value)>(value));
    }

    const Expr& Get(auto&& name) const {
      auto it = values_.find(name);
      if (it != values_.end()) {
        return it->second;
      }
      if (parent_) {
        return parent_->Get(std::forward<decltype(name)>(name));
      }
      throw std::runtime_error("Undefined variable '" + name + "'");
    }

   private:
    std::unordered_map<std::string, Expr> values_;
    std::shared_ptr<Environment> parent_;
  };

  Interpreter();
  virtual ~Interpreter() {}
  Expr Evaluate(Expr&& expr);
  Expr Evaluate(ExprList&& expr, std::shared_ptr<Environment> env = nullptr);
  std::shared_ptr<Environment> env() const { return env_; }

 private:
  struct EvalVisitor;
  std::shared_ptr<Environment> env_;
};

}  // namespace simpl

#endif  // SIMPL_INTERPRETER_H_

// Local Variables:
// compile-command : "bazel build //simpl:all"
// End:
