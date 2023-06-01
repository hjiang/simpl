// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.h"

#include <numeric>
#include <stdexcept>
#include <string>

#include "simpl/ast.h"
#include "simpl/built_in/arithmetic.h"
#include "simpl/built_in/assert.h"
#include "simpl/built_in/comparison.h"
#include "simpl/built_in/control_flow.h"
#include "simpl/built_in/io.h"
#include "simpl/built_in/logic.h"
#include "simpl/config.h"
#include "simpl/interpreter_util.h"
#include "simpl/user_fn.h"

namespace simpl {

Interpreter::Interpreter() : env_(new Environment()) {
  env_->Define("=", std::make_shared<built_in::Equals>());
  env_->Define(">", std::make_shared<built_in::GreaterThan>());
  env_->Define(">=", std::make_shared<built_in::GreaterThanOrEqualTo>());
  env_->Define("<", std::make_shared<built_in::LessThan>());
  env_->Define("<=", std::make_shared<built_in::LessThanOrEqualTo>());
  env_->Define("+", std::make_shared<built_in::Sum>());
  env_->Define("-", std::make_shared<built_in::Substract>());
  env_->Define("*", std::make_shared<built_in::Multiply>());
  env_->Define("/", std::make_shared<built_in::Divide>());
  env_->Define("%", std::make_shared<built_in::Modulo>());
  env_->Define("not", std::make_shared<built_in::Not>());
  env_->Define("print", std::make_shared<built_in::Print>());
  env_->Define("println", std::make_shared<built_in::Println>());
  env_->Define("assert", std::make_shared<built_in::Assert>());
  env_->Define("if", std::make_shared<built_in::If>());
  env_->Define("and", std::make_shared<built_in::And>());
  env_->Define("or", std::make_shared<built_in::Or>());
}

Interpreter::atom_value_type Interpreter::Evaluate(const Expr& expr) {
  expr.Accept(this);
  return last_atom_result_;
}

Interpreter::atom_value_type Interpreter::Evaluate(
    const expr_list_t& exprs, std::shared_ptr<Environment> env) {
  auto old_env = env_;
  if (env) {
    env_ = env;
  }
  for (const auto& e : exprs) {
    Evaluate(*e);
  }
  if (env) {
    env_ = old_env;
  }
  return last_atom_result_;
}

std::string Interpreter::StringifyValue(const Interpreter::atom_value_type& v) {
  if (holds<int_type>(v)) {
    return std::to_string(std::get<int_type>(v));
  }
  if (holds<float_type>(v)) {
    return std::to_string(std::get<float_type>(v));
  }
  if (holds<bool>(v)) {
    return std::get<bool>(v) ? "true" : "false";
  }
  if (holds<std::string>(v)) {
    return std::get<std::string>(v);
  }
  if (holds<Expr::Symbol>(v)) {
    return "<symbol: " + std::get<Expr::Symbol>(v).name + ">";
  }
  if (holds<nullptr_t>(v)) {
    return "nil";
  }
  if (holds<callable_ptr>(v)) {
    return "<function>";
  }
  throw std::runtime_error("Unknown atom type");
}

template <typename T>
bool Interpreter::MaybeSetAtomResult(const Expr::Atom& atom) {
  if (atom.has_value<T>()) {
    last_atom_result_ = atom.value<T>();
    return true;
  }
  return false;
}

template <>
bool Interpreter::MaybeSetAtomResult<Expr::Symbol>(const Expr::Atom& atom) {
  if (atom.has_value<Expr::Symbol>()) {
    last_atom_result_ = env_->Get(atom.value<Expr::Symbol>().name);
    return true;
  }
  return false;
}

void Interpreter::Visit(const Expr::Atom& atom) {
  if (!(MaybeSetAtomResult<int_type>(atom) ||
        MaybeSetAtomResult<float_type>(atom) ||
        MaybeSetAtomResult<bool>(atom) ||
        MaybeSetAtomResult<std::string>(atom) ||
        MaybeSetAtomResult<Expr::Symbol>(atom) ||
        MaybeSetAtomResult<std::nullptr_t>(atom))) {
    throw std::runtime_error("Unknown atom type");  // FIXME: error handling
  }
}

namespace {

class QuoteVisitor : public Expr::Visitor {
 public:
  void Visit(const Expr::Atom&) override {
    throw std::runtime_error("not implemented");
  };
  void Visit(const Expr::Def&) override {
    throw std::runtime_error("not implemented");
  };
  void Visit(const Expr::Do&) override {
    throw std::runtime_error("not implemented");
  };
  void Visit(const Expr::Fn&) override {
    throw std::runtime_error("not implemented");
  };
  void Visit(const Expr::Let&) override {
    throw std::runtime_error("not implemented");
  };
  void Visit(const Expr::List& list) override {
    value_ = std::make_shared<Expr::List>(list);
  };
  void Visit(const Expr::Quoted&) override {
    throw std::runtime_error("not implemented");
  };

  const Expr::Atom::value_type& value() const { return value_; }

 private:
  Expr::Atom::value_type value_;
};

}  // anonymous namespace

void Interpreter::Visit(const Expr::Quoted& Quoted) {
  QuoteVisitor visitor;
  Quoted.expr().Accept(&visitor);
  last_atom_result_ = visitor.value();
}

void Interpreter::Visit(const Expr::List& list) {
  if (list.exprs().empty()) {
    last_atom_result_ = nullptr;
    return;
  }
  auto result = Evaluate(*list.exprs().front());
  if (holds<callable_ptr>(result)) {
    auto callable = std::get<callable_ptr>(result);
    auto it = list.exprs().begin();
    ++it;
    expr_list_t args(it, list.exprs().end());
    last_atom_result_ = callable->Call(this, args);
  } else {
    throw std::runtime_error("Cannot apply a non-callable");
  }
}

void Interpreter::Visit(const Expr::Def& def) {
  DefVar(def.name(), Evaluate(def.expr()));
}

void Interpreter::Visit(const Expr::Let& let) {
  const auto& bindings = let.bindings();
  const auto& body = let.body();
  auto env = std::make_shared<Environment>(env_);
  for (const auto& binding : bindings) {
    env->Bind(binding.first, Evaluate(*binding.second));
  }
  Evaluate(body, env);
}

void Interpreter::Visit(const Expr::Fn& fn) {
  last_atom_result_ = std::make_shared<UserFn>(fn, env_);
}

void Interpreter::Visit(const Expr::Do& do_form) {
  for (const auto& term : do_form.exprs()) {
    Evaluate(*term);
  }
}

void Interpreter::DefVar(std::string name, atom_value_type value) {
  env_->Define(std::move(name), std::move(value));
}

}  // namespace simpl
