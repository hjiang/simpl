// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_CDT_OPS_H_
#define SIMPL_BUILT_IN_CDT_OPS_H_

#include "simpl/function.hh"

namespace simpl {
namespace built_in {

class Get : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args);
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_CDT_OPS_H_
