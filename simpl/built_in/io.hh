// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_BUILT_IN_IO_HH_
#define SIMPL_BUILT_IN_IO_HH_

#include <iostream>

#include "simpl/function.hh"

namespace simpl {

namespace built_in {

inline void PrintValues(const Function::args_type& values) {
  for (const auto& value : values) {
    std::cout << value << " ";
  }
}

class Print : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override {
    PrintValues(args);
    return Expr{nullptr};
  }
};

class Println : public Function {
 private:
  Expr FnCall(Interpreter*, args_type&& args) override {
    PrintValues(args);
    std::cout << std::endl;
    return Expr{nullptr};
  }
};

}  // namespace built_in

}  // namespace simpl

#endif  // SIMPL_BUILT_IN_IO_HH_
