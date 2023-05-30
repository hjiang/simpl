// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_PARSER_H_
#define SIMPL_PARSER_H_

#include <initializer_list>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <variant>

#include "simpl/ast.h"
#include "simpl/token.h"

namespace simpl {

using token_list_t = std::list<Token>;

class Parser {
 public:
  explicit Parser(const token_list_t& tokens)
      : tokens_(tokens), current_(tokens_.begin()) {}
  expr_list_t Parse();

  class ParseError : std::runtime_error {
   public:
    explicit ParseError(const std::string& msg) : std::runtime_error(msg) {}
  };

 private:
  ParseError Error(const Token& token, const std::string& msg);
  expr_ptr_t ParseAnd();
  expr_ptr_t ParseAtom();
  expr_ptr_t ParseDef();
  expr_ptr_t ParseDefn();
  expr_ptr_t ParseDo();
  expr_ptr_t ParseExpr();
  expr_ptr_t ParseFn();
  expr_ptr_t ParseIf();
  expr_ptr_t ParseLet();
  expr_ptr_t ParseList();
  expr_ptr_t ParseOr();
  Expr::Fn::param_list_t ParseParamList();
  Expr::Let::binding_list_t ParseBindings();
  Expr::Let::binding_t ParseBinding();
  expr_list_t ParseExprs();
  bool Match(Token::Type type);
  bool Check(Token::Type type) const;
  const Token& Consume(Token::Type type, const std::string& msg);
  const Token& Advance();
  bool AtEnd() const { return Peek().type == Token::kEof; }
  const Token& Peek() const { return *current_; }
  const Token& Previous() const { return *(std::prev(current_)); }
  const token_list_t tokens_;
  token_list_t::const_iterator current_;
};

}  // namespace simpl

#endif  // SIMPL_PARSER_H_

// Local Variables:
// compile-command : "bazel test //simpl:all"
// End:
