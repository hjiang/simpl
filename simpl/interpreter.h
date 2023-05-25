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

#include "simpl/parser.h"

namespace simpl {

class Callable;

class Interpreter : public Expr::Visitor {
 public:
  struct Symbol {
    std::string name;
  };

  using callable_ptr = std::shared_ptr<Callable>;
  using atom_value_type = std::variant<int_type, float_type, bool, std::string,
                                       Symbol, std::nullptr_t, callable_ptr>;

  class Environment {
   public:
    explicit Environment(std::shared_ptr<Environment> parent = nullptr)
        : values_(), parent_(parent) {}

    void Define(std::string name, atom_value_type value) {
      if (parent_) {
        return parent_->Define(std::move(name), std::move(value));
      } else {
        values_[std::move(name)] = std::move(value);
      }
    }

    void Bind(std::string name, atom_value_type value) {
      values_[std::move(name)] = std::move(value);
    }

    const atom_value_type& Get(const std::string& name) const {
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
    std::unordered_map<std::string, atom_value_type> values_;
    std::shared_ptr<Environment> parent_;
  };

  using function_type =
      std::function<atom_value_type(const std::vector<atom_value_type>&)>;
  using fn_args_type = const std::vector<Interpreter::atom_value_type>&;

  Interpreter();
  virtual ~Interpreter() {}
  void Visit(const Expr::Atom& atom) override;
  void Visit(const Expr::List& list) override;
  void Visit(const Expr::Def& expr) override;
  void Visit(const Expr::Let& expr) override;
  void Visit(const Expr::If& expr) override;
  void Visit(const Expr::Fn& expr) override;
  atom_value_type Evaluate(const Expr& expr);
  atom_value_type Evaluate(const std::list<std::unique_ptr<const Expr>>& expr,
                           std::shared_ptr<Environment> env = nullptr);
  static std::string StringifyValue(const atom_value_type& value);

  std::shared_ptr<Environment> env() const { return env_; }

 private:
  template <typename T>
  bool MaybeSetAtomResult(const Expr::Atom& atom);
  void DefVar(std::string name, atom_value_type value);

  atom_value_type last_atom_result_;
  std::shared_ptr<Environment> env_;
};

}  // namespace simpl

#endif  // SIMPL_INTERPRETER_H_

// Local Variables:
// compile-command : "bazel build //simpl:all"
// End:
