// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/ast.h"

namespace simpl {

void Atom::Accept(ExprVisitor* visitor) const { visitor->Visit(*this); }
void List::Accept(ExprVisitor* visitor) const { visitor->Visit(*this); }
void Vector::Accept(ExprVisitor* visitor) const { visitor->Visit(*this); }
void Quoted::Accept(ExprVisitor* visitor) const { visitor->Visit(*this); }

}  // namespace simpl
