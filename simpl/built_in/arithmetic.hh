// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_ARITHMETIC_HH_
#define SIMPL_BUILT_IN_ARITHMETIC_HH_

#include "simpl/function.hh"
#include "simpl/interpreter.hh"

namespace simpl {

namespace built_in {

class Sum : public Function {
 private:
  Expr FnCall(Interpreter* interpreter, args_type&& args) override;
};

class Substract : public Function {
 private:
  Expr FnCall(Interpreter* interpreter, args_type&& args) override;
};

class Multiply : public Function {
 private:
  Expr FnCall(Interpreter* interpreter, args_type&& args) override;
};

class Divide : public Function {
 private:
  Expr FnCall(Interpreter* interpreter, args_type&& args) override;
};

class Modulo : public Function {
 private:
  Expr FnCall(Interpreter* interpreter, args_type&& args) override;
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_ARITHMETIC_HH_
