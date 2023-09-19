// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_COMPARISON_HH_
#define SIMPL_BUILT_IN_COMPARISON_HH_

#include <vector>

#include "simpl/function.hh"
#include "simpl/interpreter.hh"

namespace simpl {

namespace built_in {

class Equals : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class GreaterThan : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class GreaterThanOrEqualTo : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class LessThan : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

class LessThanOrEqualTo : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_COMPARISON_HH_
