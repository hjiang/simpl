// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <memory>
#include <unordered_map>
#include <variant>

#include "oxid/parser.h"

namespace oxid {

class Interpreter : public Expr::Visitor {
 public:
  struct Symbol {
    std::string name;
  };

  using atom_value_type =
      std::variant<int_type, float_type, bool, std::string, Symbol, nullptr_t>;

  class Environment {
   public:
    explicit Environment(std::unique_ptr<Environment>&& parent = nullptr)
        : values_(), parent_(parent.release()) {}

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

    atom_value_type Get(const std::string& name) const {
      auto it = values_.find(name);
      if (it != values_.end()) {
        return it->second;
      }
      if (parent_) {
        return parent_->Get(name);
      }
      throw std::runtime_error("Undefined variable '" + name + "'");
    }

    void ResetParent(std::unique_ptr<Environment>&& parent) {
      parent_ = std::move(parent);
    }

    std::unique_ptr<Environment>&& ReleaseParent() {
      return std::move(parent_);
    }

   private:
    std::unordered_map<std::string, atom_value_type> values_;
    std::unique_ptr<Environment> parent_;
  };

  using function_type =
      std::function<atom_value_type(const std::vector<atom_value_type>&)>;
  using fn_args_type = const std::vector<Interpreter::atom_value_type>&;

  Interpreter() : env_(new Environment()) {}
  virtual ~Interpreter() {}
  virtual void Visit(const Expr::Atom& atom) override;
  virtual void Visit(const Expr::List& list) override;
  virtual void Visit(const Expr::Def& expr) override;
  virtual void Visit(const Expr::Let& expr) override;
  virtual void Visit(const Expr::If& expr) override;
  atom_value_type evaluate(const Expr& expr);
  atom_value_type evaluate(const std::list<std::unique_ptr<const Expr>>& expr);
  static std::string StringifyValue(const atom_value_type& value);

 private:
  template <typename T>
  bool MaybeSetAtomResult(const Expr::Atom& atom);
  void DefVar(std::string name, atom_value_type value);

  atom_value_type last_atom_result_;
  std::unique_ptr<Environment> env_;

  static std::unordered_map<std::string, function_type> built_in_functions_;
  static bool IsBuiltInFn(const std::string& name);
};

}  // namespace oxid

#endif  // INTERPRETER_H_

// Local Variables:
// compile-command : "bazel build //oxid:all"
// End:
