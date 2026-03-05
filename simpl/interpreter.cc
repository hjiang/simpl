// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.hh"

#include <iterator>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>

#include "simpl/ast.hh"
#include "simpl/built_in/arithmetic.hh"
#include "simpl/built_in/assert.hh"
#include "simpl/built_in/cdt_ops.hh"
#include "simpl/built_in/comparison.hh"
#include "simpl/built_in/control_flow.hh"
#include "simpl/built_in/def.hh"
#include "simpl/built_in/eval.hh"
#include "simpl/built_in/fn.hh"
#include "simpl/built_in/io.hh"
#include "simpl/built_in/logic.hh"
#include "simpl/built_in/sequence.hh"
#include "simpl/config.hh"
#include "simpl/interpreter_util.hh"
#include "simpl/user_fn.hh"
#include "simpl/util.hh"

namespace simpl {

struct Interpreter::EvalVisitor {
  Interpreter* interpreter;
  Expr operator()(auto&& expr) { return std::forward<Expr>(expr); }
  Expr operator()(TailCall&& tc) { return std::move(tc); }
  Expr operator()(Quoted&& expr) { return expr.expr(); }
  Expr operator()(Symbol&& expr) {
    return interpreter->env_->Get(std::move(expr.name));
  }
  Expr operator()(List&& list) {
    if (list.empty()) {
      return Expr{nullptr};
    }

    bool in_tail = std::exchange(interpreter->tail_position_, false);

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

      // Tail-position non-lazy UserFn: evaluate args now, return TailCall
      // sentinel for the trampoline to iterate without growing the C++ stack.
      auto fn = std::dynamic_pointer_cast<UserFn>(callable);
      if (in_tail && fn && !fn->is_lazy()) {
        ExprList evaluated;
        for (auto& arg : args) {
          evaluated.push_back(interpreter->Evaluate(std::move(arg)));
        }
        return TailCall{callable, std::move(evaluated)};
      }

      // Only If reads tail_position_ to decide whether to restore it for its
      // branches. Let uses EvaluateBody internally. All other callables
      // (And, Or, *, +, eval, etc.) must NOT see tail_position_=true because
      // they evaluate sub-expressions that are not in tail position.
      if (in_tail && std::dynamic_pointer_cast<built_in::If>(callable)) {
        interpreter->set_tail_position(true);
      }

      auto call_result = callable->Call(interpreter, std::move(args));

      // Non-tail context: run trampoline loop until a real value is returned.
      if (!in_tail) {
        while (auto* tc = std::get_if<TailCall>(&call_result)) {
          auto tc_fn = std::dynamic_pointer_cast<Function>(tc->callable);
          if (tc_fn) {
            call_result =
                tc_fn->CallEvaluated(interpreter, std::move(tc->args));
          } else {
            call_result = tc->callable->Call(interpreter, std::move(tc->args));
          }
        }
      }
      return call_result;
    } else {
      throw std::runtime_error("Cannot apply a non-callable");
    }
  }
};

Interpreter::Interpreter() : env_(std::make_shared<Environment>()) {
  env_->Define("=", std::make_unique<built_in::Equals>());
  env_->Define(">", std::make_unique<built_in::GreaterThan>());
  env_->Define(">=", std::make_unique<built_in::GreaterThanOrEqualTo>());
  env_->Define("<", std::make_unique<built_in::LessThan>());
  env_->Define("<=", std::make_unique<built_in::LessThanOrEqualTo>());
  env_->Define("+", std::make_unique<built_in::Sum>());
  env_->Define("-", std::make_unique<built_in::Subtract>());
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

Expr Interpreter::Evaluate(const ExprList& exprs,
                           std::shared_ptr<Environment> env) {
  return Evaluate(ExprList{exprs}, std::move(env));
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

Expr Interpreter::EvaluateBody(const ExprList& exprs,
                               std::shared_ptr<Environment> env) {
  auto old_env = env_;
  if (env) env_ = env;
  auto restore = defer([this, env, old_env]() {
    if (env) this->env_ = old_env;
  });

  Expr result{nullptr};
  for (auto it = exprs.begin(); it != exprs.end();) {
    auto next = std::next(it);
    if (next == exprs.end()) tail_position_ = true;  // mark last expr
    result = Evaluate(Expr{*it});
    tail_position_ = false;  // always reset after each eval
    it = next;
  }
  return result;
}

}  // namespace simpl
