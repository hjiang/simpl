#include "interpreter.h"

namespace oxid {

Interpreter::result_type Interpreter::evaluate(std::shared_ptr<Expr> expr) {
  expr->Accept(this);
  return result_;
}

template <typename T>
bool Interpreter::MaybeSetResult(const Expr::Atom& atom) {
  if (atom.has_value<T>()) {
    result_ = atom.value<T>();
    return true;
  }
  return false;
}

void Interpreter::Visit(const Expr::Atom& atom) {
  if (!(MaybeSetResult<long>(atom) || MaybeSetResult<double>(atom) ||
        MaybeSetResult<bool>(atom) || MaybeSetResult<std::string>(atom))) {
    throw std::runtime_error("Unknown atom type");
  }
}

void Interpreter::Visit(const Expr::List& list) {}

}  // namespace oxid
