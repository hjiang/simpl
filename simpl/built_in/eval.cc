// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/eval.hh"

#include "simpl/built_in/util.h"
#include "simpl/interpreter.h"

namespace simpl {

namespace built_in {

Expr Eval::FnCall(Interpreter* interpreter, const args_type& args) {
  CheckArity("eval", args, 1);
  return interpreter->Evaluate(args.front());
}

}  // namespace built_in

}  // namespace simpl
