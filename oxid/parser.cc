#include "parser.h"

#include <memory>

namespace oxid {

std::shared_ptr<Expr> Parser::Parse() {
  return std::make_shared<Expr::Atom>(1);
}

void Expr::Atom::Accept(ExprVisitor* visitor) const { visitor->Visit(*this); }

}  // namespace oxid
