// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_FN_H_
#define SIMPL_BUILT_IN_FN_H_

#include "simpl/ast.h"
#include "simpl/callable.h"

namespace simpl {
namespace built_in {

class Fn : public Callable {
 public:
  virtual ~Fn() = default;
  Atom::value_type Call(Interpreter* interpreter,
                        const expr_list_t& exprs) override;
};

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_FN_H_
