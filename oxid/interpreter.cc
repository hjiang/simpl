#include "interpreter.h"

#include <numeric>
#include <stdexcept>
#include <string>

#include "arithmetic.h"
#include "interpreter_util.h"
#include "parser.h"

namespace oxid {

std::unordered_map<std::string, Interpreter::function_type>
    Interpreter::built_in_functions_{
        {"+",
         [](const std::vector<Interpreter::atom_value_type>& args)
             -> Interpreter::atom_value_type {
           Interpreter::atom_value_type result = 0;
           for (auto arg : args) {
             result = result + arg;
           }
           return result;
         }},
        {"-",
         [](const std::vector<Interpreter::atom_value_type>& args)
             -> Interpreter::atom_value_type {
           auto i = args.begin();
           Interpreter::atom_value_type result = *i++;
           for (; i != args.end(); ++i) {
             result = result - *i;
           }
           return args.size() > 1 ? result : 0 - result;
         }},
        {"*",
         [](const std::vector<Interpreter::atom_value_type>& args)
             -> Interpreter::atom_value_type {
           Interpreter::atom_value_type result = 1;
           for (auto arg : args) {
             result = result * arg;
           }
           return result;
         }},
        {"/",
         [](const std::vector<Interpreter::atom_value_type>& args)
             -> Interpreter::atom_value_type {
           auto i = args.begin();
           Interpreter::atom_value_type result = *i++;
           for (; i != args.end(); ++i) {
             result = result / *i;
           }
           return result;
         }},
        {"%",
         [](const std::vector<Interpreter::atom_value_type>& args)
             -> Interpreter::atom_value_type {
           auto i = args.begin();
           Interpreter::atom_value_type result = *i++;
           for (; i != args.end(); ++i) {
             result = result % *i;
           }
           return result;
         }}};

Interpreter::atom_value_type Interpreter::evaluate(const Expr& expr) {
  expr.Accept(this);
  return last_atom_result_;
}

Interpreter::atom_value_type Interpreter::evaluate(
    const std::list<std::unique_ptr<const Expr>>& exprs) {
  for (const auto& e : exprs) {
    evaluate(*e);
  }
  return last_atom_result_;
}

std::string Interpreter::StringifyValue(const Interpreter::atom_value_type& v) {
  if (holds<long>(v)) {
    return std::to_string(std::get<long>(v));
  }
  if (holds<double>(v)) {
    return std::to_string(std::get<double>(v));
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

bool Interpreter::IsBuiltInFn(const std::string& name) {
  return built_in_functions_.find(name) != built_in_functions_.end();
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
    auto name = atom.value<Expr::Symbol>().name;
    if (IsBuiltInFn(name)) {
      last_atom_result_ = atom_value_type(Symbol{.name = name});
    } else {
      last_atom_result_ = env_->Get(name);
    }
    return true;
  }
  return false;
};

void Interpreter::Visit(const Expr::Atom& atom) {
  if (!(MaybeSetAtomResult<long>(atom) || MaybeSetAtomResult<double>(atom) ||
        MaybeSetAtomResult<bool>(atom) ||
        MaybeSetAtomResult<std::string>(atom) ||
        MaybeSetAtomResult<Expr::Symbol>(atom))) {
    throw std::runtime_error("Unknown atom type");  // FIXME: error handling
  }
}

void Interpreter::Visit(const Expr::List& list) {
  list.exprs().front()->Accept(this);
  if (!std::holds_alternative<Symbol>(last_atom_result_)) {
    throw std::runtime_error("First element of list must be a symbol");
  }
  auto name = std::get<Symbol>(last_atom_result_).name;
  auto fit = built_in_functions_.find(name);
  if (fit == built_in_functions_.end()) {
    throw std::runtime_error("Unknown function: " +
                             name);  // FIXME: error handling
  }
  auto func = fit->second;
  std::vector<atom_value_type> args;
  auto it = list.exprs().begin();
  ++it;
  for (; it != list.exprs().end(); ++it) {
    (*it)->Accept(this);
    args.push_back(last_atom_result_);
  }
  last_atom_result_ = func(args);
}

void Interpreter::Visit(const Expr::Def& def) {
  def.expr().Accept(this);
  DefVar(def.name(), last_atom_result_);
}

void Interpreter::Visit(const Expr::Let& let) {
  const auto& bindings = let.bindings();
  const auto& body = let.body();
  auto env = std::make_unique<Environment>();
  for (const auto& binding : bindings) {
    binding.second->Accept(this);
    env->Bind(binding.first, last_atom_result_);
  }
  env->ResetParent(std::move(env_));
  env_ = std::move(env);
  for (const auto& expr : body) {
    expr->Accept(this);
  }
  env_ = env_->ReleaseParent();
}

static bool IsTruthy(const Interpreter::atom_value_type& v) {
  if (holds<bool>(v)) {
    return std::get<bool>(v);
  } else if (holds<nullptr_t>(v)) {
    return false;
  } else {
    return true;
  }
}

void Interpreter::Visit(const Expr::If& if_expr) {
  if_expr.cond().Accept(this);
  if (IsTruthy(last_atom_result_)) {
    if_expr.then().Accept(this);
  } else {
    if_expr.otherwise().Accept(this);
  }
}

void Interpreter::DefVar(std::string name, atom_value_type value) {
  env_->Define(std::move(name), std::move(value));
}

}  // namespace oxid
