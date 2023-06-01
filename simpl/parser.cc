// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/parser.h"

#include <memory>
#include <string>

#include "simpl/config.h"
#include "simpl/error.h"

namespace simpl {

static void Error(const Token& token, const std::string& message) {
  if (token.type == Token::Type::kEof) {
    Report(token.line, " at end", message);
  } else {
    Report(token.line, " at '" + token.lexeme + "'", message);
  }
}

Parser::ParseError Parser::Error(const Token& token,
                                 const std::string& message) {
  simpl::Error(token, message);
  return ParseError(message);
}

const Token& Parser::Consume(Token::Type type, const std::string& message) {
  if (Check(type)) return Advance();
  throw Error(Peek(), message);
}

expr_list_t Parser::Parse() {
  expr_list_t exprs;
  while (!AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  return exprs;
}

expr_ptr_t Parser::ParseAtom() {
  auto atom = Advance();
  switch (atom.type) {
    case Token::Type::kInteger:
      return std::make_unique<Expr::Atom>(std::get<int_type>(atom.literal));
      break;
    case Token::Type::kString:
      return std::make_unique<Expr::Atom>(std::get<std::string>(atom.literal));
      break;
    case Token::Type::kFloat:
      return std::make_unique<Expr::Atom>(std::get<float_type>(atom.literal));
      break;
    case Token::Type::kFalse:
      return std::make_unique<Expr::Atom>(false);
      break;
    case Token::Type::kTrue:
      return std::make_unique<Expr::Atom>(true);
      break;
    case Token::Type::kMinus:
    case Token::Type::kPlus:
    case Token::Type::kSlash:
    case Token::Type::kStar:
    case Token::Type::kPercent:
    case Token::Type::kEqual:
    case Token::Type::kBang:
    case Token::Type::kBangEqual:
    case Token::Type::kGreater:
    case Token::Type::kGreaterEqual:
    case Token::Type::kLess:
    case Token::Type::kLessEqual:
    case Token::Type::kPrint:
    case Token::Type::kVar:
    case Token::Type::kWhile:
    case Token::Type::kSymbol:
      return std::make_unique<Expr::Atom>(Expr::Symbol{atom.lexeme});
      break;
    case Token::Type::kNil:
      return std::make_unique<Expr::Atom>(nullptr);
      break;
    default:
      throw Error(atom, "Unexpected token");
  }
}

expr_ptr_t Parser::ParseExpr() {
  if (Match(Token::Type::kLeftParen)) {
    if (Match(Token::Type::kDef)) {
      return ParseDef();
    }
    if (Match(Token::Type::kLet)) {
      return ParseLet();
    }
    if (Match(Token::Type::kFn)) {
      return ParseFn();
    }
    if (Match(Token::Type::kDefn)) {
      return ParseDefn();
    }
    if (Match(Token::Type::kDo)) {
      return ParseDo();
    }
    return ParseList();
  } else {
    return ParseAtom();
  }
}

Expr::Fn::param_list_t Parser::ParseParamList() {
  Expr::Fn::param_list_t args;
  Consume(Token::Type::kLeftBracket, "Expect '[' before parameters.");
  while (!Check(Token::Type::kRightBracket) && !AtEnd()) {
    auto token = Advance();
    if (token.type != Token::Type::kSymbol) {
      throw Error(token, "Expected symbol as parameter name");
    }
    args.push_back(token.lexeme);
  }
  Consume(Token::Type::kRightBracket, "Expect ']' after parameters.");
  return args;
}

expr_ptr_t Parser::ParseFn() {
  auto params = ParseParamList();
  auto body = ParseExprs();
  Consume(Token::Type::kRightParen, "Expect ')' after function.");
  return std::make_unique<Expr::Fn>(std::move(params), std::move(body));
}

expr_ptr_t Parser::ParseDef() {
  auto token = Advance();
  if (token.type != Token::Type::kSymbol) {
    throw Error(token, "Expected symbol after def");
  }
  auto name = token.lexeme;
  auto expr = ParseExpr();
  Consume(Token::Type::kRightParen, "Expect ')' after definition.");
  return std::make_unique<const Expr::Def>(std::move(name), std::move(expr));
}

expr_ptr_t Parser::ParseDefn() {
  auto token = Advance();
  if (token.type != Token::Type::kSymbol) {
    throw Error(token, "Expected symbol after defn");
  }
  return std::make_unique<const Expr::Def>(std::move(token.lexeme), ParseFn());
}

Expr::Let::binding_t Parser::ParseBinding() {
  auto token = Advance();
  if (token.type != Token::Type::kSymbol) {
    throw Error(token, "Expected symbol as binding target");
  }
  return std::make_pair(std::move(token.lexeme), ParseExpr());
}

Expr::Let::binding_list_t Parser::ParseBindings() {
  Expr::Let::binding_list_t bindings;
  Consume(Token::Type::kLeftBracket, "Expect '[' before bindings.");
  while (!Check(Token::Type::kRightBracket) && !AtEnd()) {
    bindings.push_back(ParseBinding());
  }
  Consume(Token::Type::kRightBracket, "Expect ']' after bindings.");
  return bindings;
}

expr_ptr_t Parser::ParseLet() {
  Expr::Let::binding_list_t bindings(ParseBindings());
  expr_list_t body(ParseExprs());
  Consume(Token::Type::kRightParen, "Expect ')' at end of LET form.");
  return std::make_unique<const Expr::Let>(std::move(bindings),
                                           std::move(body));
}

expr_list_t Parser::ParseExprs() {
  expr_list_t exprs;
  while (!Check(Token::Type::kRightParen) && !AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  return exprs;
}

expr_ptr_t Parser::ParseDo() {
  auto terms = ParseExprs();
  Consume(Token::Type::kRightParen, "Expect ')' at end of 'do' form.");
  return std::make_unique<Expr::Do>(std::move(terms));
}

expr_ptr_t Parser::ParseList() {
  expr_list_t exprs;
  while (!Check(Token::Type::kRightParen) && !AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  Consume(Token::Type::kRightParen, "Expect ')' after list.");
  return std::make_unique<Expr::List>(std::move(exprs));
}

bool Parser::Match(Token::Type type) {
  if (Check(type)) {
    Advance();
    return true;
  }
  return false;
}

bool Parser::Check(Token::Type type) const {
  return AtEnd() ? false : Peek().type == type;
}

const Token& Parser::Advance() {
  if (!AtEnd()) ++current_;
  return Previous();
}

}  // namespace simpl

// Local Variables:
// compile-command : "bazel test //simpl:parser_test"
// End:
