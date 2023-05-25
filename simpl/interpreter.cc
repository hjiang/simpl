// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.h"

#include <numeric>
#include <stdexcept>
#include <string>

#include "simpl/arithmetic.h"
#include "simpl/comparison.h"
#include "simpl/config.h"
#include "simpl/interpreter_util.h"
#include "simpl/logic.h"
#include "simpl/parser.h"
#include "simpl/user_fn.h"

namespace simpl {

Interpreter::Interpreter() : env_(new Environment()) {
  env_->Define("=", std::make_shared<builtin_fn::Equals>());
  env_->Define(">", std::make_shared<builtin_fn::GreaterThan>());
  env_->Define(">=", std::make_shared<builtin_fn::GreaterThanOrEqualTo>());
  env_->Define("<", std::make_shared<builtin_fn::LessThan>());
  env_->Define("<=", std::make_shared<builtin_fn::LessThanOrEqualTo>());
  env_->Define("+", std::make_shared<builtin_fn::Sum>());
  env_->Define("-", std::make_shared<builtin_fn::Substract>());
  env_->Define("*", std::make_shared<builtin_fn::Multiply>());
  env_->Define("/", std::make_shared<builtin_fn::Divide>());
  env_->Define("%", std::make_shared<builtin_fn::Modulo>());
  env_->Define("not", std::make_shared<builtin_fn::Not>());
}

Interpreter::atom_value_type Interpreter::Evaluate(const Expr& expr) {
  expr.Accept(this);
  return last_atom_result_;
}

Interpreter::atom_value_type Interpreter::Evaluate(
    const std::list<std::unique_ptr<const Expr>>& exprs,
    std::shared_ptr<Environment> env) {
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
  if (holds<Symbol>(v)) {
    return std::get<Symbol>(v).name;
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

void Interpreter::Visit(const Expr::List& list) {
  auto result = Evaluate(*list.exprs().front());
  if (holds<callable_ptr>(result)) {
    auto callable = std::get<callable_ptr>(result);
    auto it = list.exprs().begin();
    ++it;
    std::list<atom_value_type> args;
    for (; it != list.exprs().end(); ++it) {
      args.push_back(Evaluate(**it));
    }
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

void Interpreter::Visit(const Expr::If& if_expr) {
  if (IsTruthy(Evaluate(if_expr.cond()))) {
    Evaluate(if_expr.then());
  } else {
    Evaluate(if_expr.otherwise());
  }
}

void Interpreter::Visit(const Expr::Fn& fn) {
  last_atom_result_ = std::make_shared<UserFn>(fn);
}

void Interpreter::DefVar(std::string name, atom_value_type value) {
  env_->Define(std::move(name), std::move(value));
}

}  // namespace simpl
