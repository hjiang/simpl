// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_CALLABLE_H_
#define SIMPL_CALLABLE_H_

#include <ostream>

#include "simpl/ast.h"

namespace simpl {

class Interpreter;

class Callable {
 public:
  using args_type = std::list<Expr>;
  using args_iterator = args_type::iterator;
  virtual ~Callable() {}
  virtual Expr Call(Interpreter*, args_type&& exprs) = 0;
};

std::ostream& operator<<(std::ostream& os, const Callable&);

}  // namespace simpl

#endif  // SIMPL_CALLABLE_H_
