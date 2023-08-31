// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.h"

#include <iterator>
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
#include "simpl/built_in/eval.hh"
#include "simpl/built_in/fn.h"
#include "simpl/built_in/io.h"
#include "simpl/built_in/logic.h"
#include "simpl/built_in/sequence.h"
#include "simpl/config.h"
#include "simpl/interpreter_util.h"
#include "simpl/user_fn.h"
#include "simpl/util.hh"

namespace simpl {

struct Interpreter::EvalVisitor {
  Interpreter* interpreter;
  Expr operator()(auto&& expr) { return std::forward<Expr>(expr); }
  Expr operator()(Quoted&& expr) { return std::move(expr.expr()); }
  Expr operator()(Symbol&& expr) {
    return interpreter->env_->Get(std::move(expr.name));
  }
  Expr operator()(List&& list) {
    if (list.empty()) {
      return Expr{nullptr};
    }
    auto result = interpreter->Evaluate(std::move(list.front()));
    if (holds<Keyword>(result)) {
      if (list.size() != 2) {
        throw std::runtime_error("Keyword expects 1 argument.");
      }
      auto m = interpreter->Evaluate(std::move(list.back()));
      return std::get<Map>(m).at(result);
    } else if (holds<callable_ptr_t>(result)) {
      auto callable = std::get<callable_ptr_t>(result);
      auto it = list.begin();
      ++it;
      ExprList args;
      std::move(it, list.end(), std::back_inserter(args));
      return callable->Call(interpreter, std::move(args));
    } else {
      throw std::runtime_error("Cannot apply a non-callable");
    }
  }
};

Interpreter::Interpreter() : env_(new Environment()) {
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
  env_->Define("lazy-fn", std::make_unique<built_in::Fn>(true));
  env_->Define("def", std::make_unique<built_in::Def>());
  env_->Define("defn", std::make_unique<built_in::Defn>());
  env_->Define("eval", std::make_unique<built_in::Eval>());
  env_->Define("let", std::make_unique<built_in::Let>());
  env_->Define("cons", std::make_unique<built_in::Cons>());
  env_->Define("head", std::make_unique<built_in::Head>());
  env_->Define("tail", std::make_unique<built_in::Tail>());
  env_->Define("get", std::make_unique<built_in::Get>());
  env_->Define("empty?", std::make_unique<built_in::Empty>());
}

Expr Interpreter::Evaluate(Expr&& expr) {
  EvalVisitor visitor{this};
  return std::visit(visitor, std::move(expr));
}

Expr Interpreter::Evaluate(ExprList&& exprs, std::shared_ptr<Environment> env) {
  auto old_env = env_;
  if (env) {
    env_ = env;
  }
  auto restore = defer([this, env, old_env]() {
    if (env) {
      this->env_ = old_env;
    }
  });
  return std::accumulate(
      make_move_iterator(exprs.begin()), make_move_iterator(exprs.end()),
      Expr(nullptr),
      [this](Expr&&, Expr&& rhs) { return Evaluate(std::move(rhs)); });
}

}  // namespace simpl
