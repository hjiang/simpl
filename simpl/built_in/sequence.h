// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_SEQUENCE_H_
#define SIMPL_BUILT_IN_SEQUENCE_H_

#include "simpl/function.h"

namespace simpl {
namespace built_in {

// TODO(hjiang): reevaluate whether vectors should support these functions.

class Cons : public Function {
 private:
  Expr FnCall(Interpreter*, const args_type& args) override;
};

class Head : public Function {
 private:
  Expr FnCall(Interpreter*, const args_type& args) override;
};

class Tail : public Function {
 private:
  Expr FnCall(Interpreter*, const args_type& args) override;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_SEQUENCE_H_
