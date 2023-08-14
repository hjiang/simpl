// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.h"

#include <numeric>
#include <stdexcept>
#include <string>

#include "simpl/ast.h"
#include "simpl/built_in/arithmetic.h"
#include "simpl/built_in/assert.h"
#include "simpl/built_in/cdt_ops.h"
#include "simpl/built_in/comparison.h"
#include "simpl/built_in/control_flow.h"
#include "simpl/built_in/def.h"
#include "simpl/built_in/fn.h"
#include "simpl/built_in/io.h"
#include "simpl/built_in/logic.h"
#include "simpl/built_in/sequence.h"
#include "simpl/config.h"
#include "simpl/interpreter_util.h"
#include "simpl/user_fn.h"

namespace simpl {

struct Interpreter::EvalVisitor {
  Interpreter* interpreter;
  void operator()(const auto& expr) { interpreter->last_value_ = expr; }
  void operator()(const Quoted& expr) {
    interpreter->last_value_ = expr.expr();
  }
  void operator()(const Symbol& expr) {
    interpreter->last_value_ = interpreter->env_->Get(expr.name);
  }
  void operator()(const List& list) {
    if (list.empty()) {
      interpreter->last_value_ = nullptr;
      return;
    }
    auto result = interpreter->Evaluate(list.front());
    if (holds<callable_ptr_t>(result)) {
      auto callable = std::get<callable_ptr_t>(result);
      auto it = list.begin();
      ++it;
      ExprList args(it, list.end());
      interpreter->last_value_ = callable->Call(interpreter, args);
    } else {
      throw std::runtime_error("Cannot apply a non-callable");
    }
  }
};

Interpreter::Interpreter() : last_value_(nullptr), env_(new Environment()) {
  env_->Define("=", std::make_unique<built_in::Equals>());
  env_->Define(">", std::make_unique<built_in::GreaterThan>());
  env_->Define(">=", std::make_unique<built_in::GreaterThanOrEqualTo>());
  env_->Define("<", std::make_unique<built_in::LessThan>());
  env_->Define("<=", std::make_unique<built_in::LessThanOrEqualTo>());
  env_->Define("+", std::make_unique<built_in::Sum>());
  env_->Define("-", std::make_unique<built_in::Substract>());
  env_->Define("*", std::make_unique<built_in::Multiply>());
  env_->Define("/", std::make_unique<built_in::Divide>());
  env_->Define("%", std::make_unique<built_in::Modulo>());
  env_->Define("not", std::make_unique<built_in::Not>());
  env_->Define("print", std::make_unique<built_in::Print>());
  env_->Define("println", std::make_unique<built_in::Println>());
  env_->Define("assert", std::make_unique<built_in::Assert>());
  env_->Define("if", std::make_unique<built_in::If>());
  env_->Define("and", std::make_unique<built_in::And>());
  env_->Define("or", std::make_unique<built_in::Or>());
  env_->Define("do", std::make_unique<built_in::Do>());
  env_->Define("fn", std::make_unique<built_in::Fn>());
  env_->Define("def", std::make_unique<built_in::Def>());
  env_->Define("defn", std::make_unique<built_in::Defn>());
  env_->Define("let", std::make_unique<built_in::Let>());
  env_->Define("cons", std::make_unique<built_in::Cons>());
  env_->Define("head", std::make_unique<built_in::Head>());
  env_->Define("tail", std::make_unique<built_in::Tail>());
  env_->Define("get", std::make_unique<built_in::Get>());
}

const Expr& Interpreter::Evaluate(const Expr& expr) {
  EvalVisitor visitor{this};
  std::visit(visitor, expr);
  return last_value_;
}

const Expr& Interpreter::Evaluate(const ExprList& exprs,
                                  std::shared_ptr<Environment> env) {
  auto old_env = env_;
  if (env) {
    env_ = env;
  }
  for (const auto& e : exprs) {
    Evaluate(e);
  }
  if (env) {
    env_ = old_env;
  }
  return last_value_;
}

}  // namespace simpl
