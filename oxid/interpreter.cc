#include "interpreter.h"

#include <numeric>
#include <stdexcept>
#include <string>

#include "parser.h"

namespace oxid {

Interpreter::atom_value_type operator+(
    const Interpreter::atom_value_type& lhs,
    const Interpreter::atom_value_type& rhs) {
  if (std::holds_alternative<long>(lhs) && std::holds_alternative<long>(rhs)) {
    return std::get<long>(lhs) + std::get<long>(rhs);
  }
  if (std::holds_alternative<long>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<long>(lhs) + std::get<double>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<long>(rhs)) {
    return std::get<double>(lhs) + std::get<long>(rhs);
  }
  if (std::holds_alternative<double>(lhs) &&
      std::holds_alternative<double>(rhs)) {
    return std::get<double>(lhs) + std::get<double>(rhs);
  }
  if (std::holds_alternative<std::string>(lhs) &&
      std::holds_alternative<std::string>(rhs)) {
    return std::get<std::string>(lhs) + std::get<std::string>(rhs);
  }
  throw std::runtime_error(
      "Invalid types for operator +");  // FIXME: error handling
}

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
         }}};

Interpreter::atom_value_type Interpreter::evaluate(std::shared_ptr<Expr> expr) {
  expr->Accept(this);
  return last_atom_result_;
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
    last_atom_result_ =
        atom_value_type(Symbol{.name = atom.value<Expr::Symbol>().name});
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

}  // namespace oxid
