// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/eval.hh"

#include <utility>

#include "simpl/built_in/util.hh"
#include "simpl/interpreter.hh"

namespace simpl {

namespace built_in {

Expr Eval::FnCall(Interpreter* interpreter, args_type&& args) {
  CheckArity("eval", args, 1);
  return interpreter->Evaluate(std::move(args.front()));
}

}  // namespace built_in

}  // namespace simpl
