// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_USER_MACRO_HH_
#define SIMPL_USER_MACRO_HH_

#include <memory>
#include <utility>

#include "simpl/user_fn.hh"

namespace simpl {

// A macro is a UserFn that always receives unevaluated args (lazy=true).
// Its result is re-evaluated by the interpreter at the call site.
class UserMacro : public UserFn {
 public:
  explicit UserMacro(FnDef&& definition,
                     std::shared_ptr<Interpreter::Environment> closure = nullptr)
      : UserFn(std::move(definition), closure, /*lazy=*/true) {}
};

}  // namespace simpl

#endif  // SIMPL_USER_MACRO_HH_
