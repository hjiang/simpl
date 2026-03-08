// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_MACRO_HH_
#define SIMPL_BUILT_IN_MACRO_HH_

#include "simpl/callable.hh"
#include "simpl/function.hh"

namespace simpl {
namespace built_in {

// (defmacro name [params] body...)
class Defmacro : public Callable {
 public:
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;
};

// (macroexpand '(macro-name args...))
class Macroexpand : public Function {
 private:
  Expr FnCall(Interpreter* interpreter, args_type&& args) override;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_MACRO_HH_
