// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/built_in/cdt_ops.hh"

#include <stdexcept>
#include <variant>

#include "simpl/built_in/util.hh"
#include "simpl/overload.hh"

namespace simpl {
namespace built_in {

Expr Get::FnCall(Interpreter*, args_type&& args) {
  CheckArity("get", args, 2);
  return std::visit(
      Overload{[](const auto&, const auto&) -> Expr {
                 throw std::runtime_error("get: invalid argument types");
               },
               [](const Map& v, const auto& key) { return v.at(Expr{key}); },
               [](const Vector& v, int_type i) {
                 return v.at(static_cast<Vector::size_type>(i));
               }},
      args.front(), args.back());
}

}  // namespace built_in
}  // namespace simpl
