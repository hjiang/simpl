// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef OXID_ARITHMETIC_H_
#define OXID_ARITHMETIC_H_

#include "oxid/interpreter.h"

namespace oxid {

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

}  // namespace oxid

#endif  // OXID_ARITHMETIC_H_
