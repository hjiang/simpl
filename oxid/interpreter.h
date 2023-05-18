#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <variant>

#include "parser.h"

namespace oxid {

class Interpreter : public Expr::Visitor {
 public:
  struct Symbol {
    std::string name;
  };

  using atom_value_type = std::variant<long, double, bool, std::string, Symbol>;
  using function_type =
      std::function<atom_value_type(const std::vector<atom_value_type>&)>;
  using fn_args_type = const std::vector<Interpreter::atom_value_type>&;

  virtual ~Interpreter() {}
  virtual void Visit(const Expr::Atom& atom) override;
  virtual void Visit(const Expr::List& list) override;
  atom_value_type evaluate(std::shared_ptr<Expr> expr);
  static std::string StringifyValue(const atom_value_type& value);

 private:
  template <typename T>
  bool MaybeSetAtomResult(const Expr::Atom& atom);
  atom_value_type last_atom_result_;
  static std::unordered_map<std::string, function_type> built_in_functions_;
  friend Interpreter::atom_value_type operator+(
      const Interpreter::atom_value_type& lhs,
      const Interpreter::atom_value_type& rhs);
};

Interpreter::atom_value_type operator+(const Interpreter::atom_value_type& lhs,
                                       const Interpreter::atom_value_type& rhs);

}  // namespace oxid

#endif  // INTERPRETER_H_

// Local Variables:
// compile-command : "bazel build //oxid:all"
// End:
