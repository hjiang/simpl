#include "interpreter.h"

namespace oxid {

Interpreter::result_type Interpreter::evaluate(std::shared_ptr<Expr> expr) {
  expr->Accept(this);
  return result_;
}

void Interpreter::Visit(const Expr::Atom& atom) {
  result_ = atom.value<long>();
}

void Interpreter::Visit(const Expr::List& list) {}

}  // namespace oxid
