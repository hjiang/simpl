// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_ASSERT_H_
#define SIMPL_BUILT_IN_ASSERT_H_

#include <stdexcept>
#include <string>

#include "simpl/callable.h"
#include "simpl/interpreter.h"
#include "simpl/interpreter_util.h"

namespace simpl {

namespace builtin_fn {

class Assert : public Callable {
 public:
  int arity() const override { return 2; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override {
    if (!IsTruthy(args.front())) {
      throw std::runtime_error(std::get<std::string>(args.back()));
    }
    return nullptr;
  }
};

}  // namespace builtin_fn

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_ASSERT_H_