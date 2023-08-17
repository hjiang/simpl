// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_CONCEPTS_HH_
#define SIMPL_CONCEPTS_HH_

#include <concepts>

namespace simpl {

template <typename T>
concept Numeric = (std::integral<T> || std::floating_point<T>) &&
                  (!std::same_as<T, bool>);

}  // namespace simpl

#endif  // SIMPL_CONCEPTS_HH_
