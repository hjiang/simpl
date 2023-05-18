#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <variant>

#include "parser.h"

namespace oxid {

class Interpreter : public Expr::Visitor {
  using result_type = std::variant<long, double, bool, std::string>;

 public:
  virtual ~Interpreter() {}
  virtual void Visit(const Expr::Atom& atom) override;
  virtual void Visit(const Expr::List& list) override;
  result_type evaluate(std::shared_ptr<Expr> expr);

 private:
  template <typename T>
  bool MaybeSetResult(const Expr::Atom& atom);
  result_type result_;
};

}  // namespace oxid

#endif  // INTERPRETER_H_

// Local Variables:
// compile-command : "bazel build //oxid:all"
// End:
