// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_UTIL_HH_
#define SIMPL_UTIL_HH_

#include <concepts>
#include <utility>

namespace simpl {

template <typename T>
concept Numeric = (std::integral<T> || std::floating_point<T>) &&
                  (!std::same_as<T, bool>);

template <typename F>
class Finalizer {
 public:
  template <typename T>
  explicit Finalizer(T&& f) : f_(f) {}
  ~Finalizer() { f_(); }

 private:
  F f_;
};

template <typename F>
Finalizer<F> defer(F&& f) {
  return Finalizer<F>(std::forward<F>(f));
}

}  // namespace simpl

#endif  // SIMPL_UTIL_HH_
