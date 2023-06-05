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

    void Define(std::string name, Expr value) {
      if (parent_) {
        return parent_->Define(std::move(name), std::move(value));
      } else {
        values_[std::move(name)] = std::move(value);
      }
    }

    void Bind(std::string name, Expr value) {
      values_[std::move(name)] = std::move(value);
    }

    const Expr& Get(const std::string& name) const {
      auto it = values_.find(name);
      if (it != values_.end()) {
        return it->second;
      }
      if (parent_) {
        return parent_->Get(name);
      }
      throw std::runtime_error("Undefined variable '" + name + "'");
    }

    void set_parent(std::shared_ptr<Environment>&& parent) { parent_ = parent; }
    std::shared_ptr<Environment> parent() const { return parent_; }

   private:
    std::unordered_map<std::string, Expr> values_;
    std::shared_ptr<Environment> parent_;
  };

  Interpreter();
  virtual ~Interpreter() {}
  Expr Evaluate(const Expr& expr);
  Expr Evaluate(const expr_list_t& expr,
                std::shared_ptr<Environment> env = nullptr);
  static std::string StringifyValue(const Expr& value);

  std::shared_ptr<Environment> env() const { return env_; }

  const Expr& last_value() const { return last_value_; }

 private:
  struct EvalVisitor;

  Expr last_value_;
  std::shared_ptr<Environment> env_;
};

}  // namespace simpl

#endif  // SIMPL_INTERPRETER_H_

// Local Variables:
// compile-command : "bazel build //simpl:all"
// End:
