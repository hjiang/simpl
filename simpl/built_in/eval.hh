// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_EVAL_HH_
#define SIMPL_BUILT_IN_EVAL_HH_

#include "simpl/function.h"

namespace simpl {
namespace built_in {

class Eval : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_EVAL_HH_
