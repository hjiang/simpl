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
    return List(expr_list_t{car});
  }
};

Expr Cons::FnCall(Interpreter*, const args_type& args) {
  CheckArity("cons", args, 2);
  return std::visit(ConsVisitor(), args.front(), args.back());
}

struct HeadVisitor {
  template <typename T>
  Expr operator()(T) const {
    throw std::runtime_error("head: invalid argument type");
  }
};

template <>
Expr HeadVisitor::operator()(list_ptr_t list) const {
  return list->Head();
}

template <>
Expr HeadVisitor::operator()(vector_ptr_t vec) const {
  return vec->Head();
}

Expr Head::FnCall(Interpreter*, const args_type& args) {
  CheckArity("head", args, 1);
  return std::visit(HeadVisitor(), args.front());
}

struct TailVisitor {
  template <typename T>
  Expr operator()(T) const {
    throw std::runtime_error("head: invalid argument type");
  }
};

template <>
Expr TailVisitor::operator()(list_ptr_t list) const {
  return list->Tail();
}

template <>
Expr TailVisitor::operator()(vector_ptr_t vec) const {
  return vec->Tail();
}

Expr Tail::FnCall(Interpreter*, const args_type& args) {
  CheckArity("tail", args, 1);
  return std::visit(TailVisitor(), args.front());
}

}  // namespace built_in
}  // namespace simpl
