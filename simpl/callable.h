// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_CALLABLE_H_
#define SIMPL_CALLABLE_H_

#include <list>

#include "simpl/ast.h"
#include "simpl/interpreter.h"

namespace simpl {

class Callable {
 public:
  virtual ~Callable() {}
  virtual Interpreter::atom_value_type Call(Interpreter*,
                                            const expr_list_t& exprs) = 0;
};

class Function : public Callable {
 public:
  using args_type = std::list<Atom::value_type>;
  Interpreter::atom_value_type Call(Interpreter*,
                                    const expr_list_t& exprs) override;

 private:
  virtual Interpreter::atom_value_type FnCall(Interpreter*,
                                              const args_type& args) = 0;
};

}  // namespace simpl

#endif  // SIMPL_CALLABLE_H_
