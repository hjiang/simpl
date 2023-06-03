// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_LOGIC_H_
#define SIMPL_BUILT_IN_LOGIC_H_

#include "simpl/callable.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace built_in {

class Not : public Function {
 private:
  Interpreter::atom_value_type FnCall(Interpreter*,
                                      const args_type& args) override {
    return Interpreter::atom_value_type(
        static_cast<bool>(!IsTruthy(args.front())));
  }
};

class Or : public Callable {
 public:
  virtual ~Or() = default;
  Atom::value_type Call(Interpreter* interpreter,
                              const expr_list_t& exprs) override;
};

class And : public Callable {
 public:
  virtual ~And() = default;
  Atom::value_type Call(Interpreter* interpreter,
                              const expr_list_t& exprs) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_LOGIC_H_
