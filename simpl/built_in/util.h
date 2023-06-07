// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_UTIL_H_
#define SIMPL_BUILT_IN_UTIL_H_

#include <stdexcept>
#include <string>

namespace simpl {
namespace built_in {

template <typename T>
void CheckArity(const std::string& name, const T& args, size_t arity) {
  if (args.size() != arity) {
    throw std::runtime_error(std::string("Wrong number of arguments to ") +
                             name);
  }
}

}  // namespace built_in
}  // namespace simpl

#endif  // SIMPL_BUILT_IN_UTIL_H_
