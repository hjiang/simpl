// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_IO_H_
#define SIMPL_IO_H_

#include <iostream>

#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

namespace builtin_fn {

inline void PrintValues(const Callable::args_type& values) {
  for (const auto& value : values) {
    std::cout << Interpreter::StringifyValue(value) << " ";
  }
}

class Print : public Callable {
 public:
  int arity() const override { return -1; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override {
    PrintValues(args);
    return nullptr;
  }
};

class Println : public Callable {
 public:
  int arity() const override { return -1; }

 private:
  Interpreter::atom_value_type CallImpl(Interpreter*,
                                        const args_type& args) override {
    PrintValues(args);
    std::cout << std::endl;
    return nullptr;
  }
};

}  // namespace builtin_fn

}  // namespace simpl

#endif  // SIMPL_IO_H_
