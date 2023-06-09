// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/cdt_ops.h"

#include <stdexcept>
#include <variant>

#include "simpl/built_in/util.h"

namespace simpl {

namespace built_in {

struct GetVisitor {
  template <typename T, typename U>
  Expr operator()(T, U) const {
    throw std::runtime_error("get: invalid argument types");
  }
};

template <>
Expr GetVisitor::operator()(vector_ptr_t v, int_type i) const {
  return v->Get(i);
}

Expr Get::FnCall(Interpreter*, const args_type& args) {
  CheckArity("get", args, 2);
  return std::visit(GetVisitor(), args.front(), args.back());
}

}  // namespace built_in

}  // namespace simpl
