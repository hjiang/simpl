// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_FUNCTION_HH_
#define SIMPL_FUNCTION_HH_

#include <utility>

#include "simpl/ast.hh"
#include "simpl/callable.hh"

namespace simpl {

class Function : public Callable {
 public:
  explicit Function(bool lazy = false) : lazy_(lazy) {}
  Expr Call(Interpreter*, args_type&& exprs) override;
  bool is_lazy() const { return lazy_; }
  // Call FnCall directly with already-evaluated args (for TCO trampoline).
  Expr CallEvaluated(Interpreter* interpreter, args_type&& args) {
    return FnCall(interpreter, std::move(args));
  }

 private:
  virtual Expr FnCall(Interpreter*, args_type&& args) = 0;
  bool lazy_;
};

}  // namespace simpl

#endif  // SIMPL_FUNCTION_HH_
