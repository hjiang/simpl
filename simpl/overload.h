// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_OVERLOAD_H_
#define SIMPL_OVERLOAD_H_

namespace simpl {

template <class... Ts>
struct Overload : Ts... {
  using Ts::operator()...;
};

template <class... Ts>
Overload(Ts...) -> Overload<Ts...>;

}  // namespace simpl

#endif  // SIMPL_OVERLOAD_H_
