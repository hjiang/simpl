// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_FN_H_
#define SIMPL_BUILT_IN_FN_H_

#include "simpl/ast.h"
#include "simpl/callable.h"

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

#endif  // SIMPL_BUILT_IN_FN_H_
