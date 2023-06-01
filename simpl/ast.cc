// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/ast.h"

namespace simpl {

void Expr::And::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }
void Expr::Atom::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }
void Expr::Def::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }
void Expr::Do::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }
void Expr::Fn::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }
void Expr::Let::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }
void Expr::List::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }
void Expr::Quoted::Accept(Expr::Visitor* visitor) const {
  visitor->Visit(*this);
}

}  // namespace simpl
