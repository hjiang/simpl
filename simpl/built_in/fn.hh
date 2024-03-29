// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_FN_HH_
#define SIMPL_BUILT_IN_FN_HH_

#include "simpl/ast.hh"
#include "simpl/callable.hh"

namespace simpl {
namespace built_in {

class Fn : public Callable {
 public:
  explicit Fn(bool lazy = false) : lazy_(lazy) {}
  virtual ~Fn() = default;
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;

 private:
  bool lazy_;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_FN_HH_
