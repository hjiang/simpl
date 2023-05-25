// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_COMPARISON_H_
#define SIMPL_COMPARISON_H_

#include <vector>

#include "simpl/interpreter.h"

namespace simpl {

Interpreter::atom_value_type Equal(
    const std::vector<Interpreter::atom_value_type>& args);

}  // namespace simpl

#endif  // SIMPL_COMPARISON_H_
