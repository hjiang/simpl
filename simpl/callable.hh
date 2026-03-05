// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_CALLABLE_HH_
#define SIMPL_CALLABLE_HH_

#include <list>
#include <ostream>

#include "simpl/ast.hh"

namespace simpl {

class Interpreter;

class Callable {
 public:
  using args_type = std::list<Expr>;
  virtual ~Callable() {}
  virtual Expr Call(Interpreter*, args_type&& exprs) = 0;
};

std::ostream& operator<<(std::ostream& os, const Callable&);

}  // namespace simpl

#endif  // SIMPL_CALLABLE_HH_
