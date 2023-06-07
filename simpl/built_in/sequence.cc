// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/sequence.h"

#include <memory>
#include <stdexcept>
#include <variant>

#include "simpl/built_in/util.h"

namespace simpl {
namespace built_in {

struct ConsVisitor {
  template <typename T, typename U>
  Expr operator()(T, U) const {
    throw std::runtime_error("cons: invalid argument type");
  }

  template <typename T>
  Expr operator()(T car, list_ptr_t cdr) const {
    return cdr->Cons(car);
  }

  template <typename T>
  Expr operator()(T car, nullptr_t) const {
    return std::make_unique<List>(expr_list_t{std::make_unique<Expr>(car)});
  }
};

Expr Cons::FnCall(Interpreter*, const args_type& args) {
  CheckArity("cons", args, 2);
  return std::visit(ConsVisitor(), args.front(), args.back());
}

}  // namespace built_in
}  // namespace simpl
