// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_ARITHMETIC_H_
#define SIMPL_ARITHMETIC_H_

#include "simpl/interpreter.h"

namespace simpl {

Interpreter::atom_value_type operator+(const Interpreter::atom_value_type& lhs,
                                       const Interpreter::atom_value_type& rhs);
Interpreter::atom_value_type operator-(const Interpreter::atom_value_type& lhs,
                                       const Interpreter::atom_value_type& rhs);
Interpreter::atom_value_type operator*(const Interpreter::atom_value_type& lhs,
                                       const Interpreter::atom_value_type& rhs);
Interpreter::atom_value_type operator/(const Interpreter::atom_value_type& lhs,
                                       const Interpreter::atom_value_type& rhs);
Interpreter::atom_value_type operator%(const Interpreter::atom_value_type& lhs,
                                       const Interpreter::atom_value_type& rhs);

}  // namespace simpl

#endif  // SIMPL_ARITHMETIC_H_
