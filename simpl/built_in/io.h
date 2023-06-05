// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_IO_H_
#define SIMPL_BUILT_IN_IO_H_

#include <iostream>

#include "simpl/callable.h"
#include "simpl/interpreter.h"

namespace simpl {

namespace built_in {

inline void PrintValues(const Function::args_type& values) {
  for (const auto& value : values) {
    std::cout << Interpreter::StringifyValue(value) << " ";
  }
}

class Print : public Function {
 private:
  Expr FnCall(Interpreter*, const args_type& args) override {
    PrintValues(args);
    return nullptr;
  }
};

class Println : public Function {
 private:
  Expr FnCall(Interpreter*, const args_type& args) override {
    PrintValues(args);
    std::cout << std::endl;
    return nullptr;
  }
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_IO_H_
