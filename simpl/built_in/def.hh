// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_DEF_HH_
#define SIMPL_BUILT_IN_DEF_HH_

#include "simpl/callable.hh"

namespace simpl {
namespace built_in {

class Def : public Callable {
 public:
  virtual ~Def() = default;
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;
};

class Defn : public Callable {
 public:
  virtual ~Defn() = default;
  Expr Call(Interpreter* interpreter, args_type&& exprs) override;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_DEF_HH_
