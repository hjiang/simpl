#ifndef ARITHMETIC_H_
#define ARITHMETIC_H_

#include "interpreter.h"

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

#endif  // ARITHMETIC_H_
