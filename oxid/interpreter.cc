#include "interpreter.h"

#include <numeric>
#include <stdexcept>
#include <string>

#include "parser.h"

namespace oxid {

template <typename T>
static bool holds(Interpreter::atom_value_type value) {
  return std::holds_alternative<T>(value);
}

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
         }},
        {"-",
         [](Interpreter::fn_args_type args) -> Interpreter::atom_value_type {
           if (args.size() > 2 || args.size() < 1) {
             throw std::runtime_error(
                 "Invalid number of arguments for operator -");  // FIXME: error
                                                                 // handling
           }
           auto lhs = args[0];
           if (args.size() == 1) {
             if (holds<long>(lhs)) {
               return -std::get<long>(lhs);
             }
             if (holds<double>(lhs)) {
               return -std::get<double>(lhs);
             }
           }
           auto rhs = args[1];
           if (holds<long>(lhs) && holds<long>(rhs)) {
             return std::get<long>(lhs) - std::get<long>(rhs);
           }
           if (holds<double>(lhs) && holds<double>(rhs)) {
             return std::get<double>(lhs) - std::get<double>(rhs);
           }
           if (holds<double>(lhs) && holds<long>(rhs)) {
             return std::get<double>(lhs) - std::get<long>(rhs);
           }
           if (holds<long>(lhs) && holds<double>(rhs)) {
             return std::get<long>(lhs) - std::get<double>(rhs);
           }
           throw std::runtime_error(
               "Invalid types for operator -");  // FIXME: error handling
         }}};

Interpreter::atom_value_type Interpreter::evaluate(
    const std::unique_ptr<const Expr>& expr) {
  expr->Accept(this);
  return last_atom_result_;
}

Interpreter::atom_value_type Interpreter::evaluate(
    const std::list<std::unique_ptr<const Expr>>& exprs) {
  for (auto& e : exprs) {
    evaluate(e);
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
