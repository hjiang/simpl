// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_CALLABLE_H_
#define SIMPL_CALLABLE_H_

#include <list>

#include "simpl/interpreter.h"

namespace simpl {

class Callable {
 public:
  using args_type = std::list<Interpreter::atom_value_type>;
  virtual ~Callable() {}
  Interpreter::atom_value_type Call(Interpreter*, const args_type& args);
  virtual int arity() const = 0;

 private:
  virtual Interpreter::atom_value_type CallImpl(
      Interpreter*, const args_type& args) = 0;
};

}  // namespace simpl

#endif  // SIMPL_CALLABLE_H_
