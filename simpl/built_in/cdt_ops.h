// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_CDT_OPS_H_
#define SIMPL_BUILT_IN_CDT_OPS_H_

#include "simpl/ast.h"
#include "simpl/function.h"

namespace simpl {
namespace built_in {

class Get : public Function {
 private:
  Expr FnCall(Interpreter*, const args_type& args) override;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_CDT_OPS_H_
