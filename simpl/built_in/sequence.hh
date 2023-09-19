// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_SEQUENCE_HH_
#define SIMPL_BUILT_IN_SEQUENCE_HH_

#include "simpl/function.hh"

namespace simpl {
namespace built_in {

// TODO(hjiang): reevaluate whether vectors should support these functions.

class Cons : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class Head : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class Tail : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class Empty : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_SEQUENCE_HH_
