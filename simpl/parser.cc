// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/parser.hh"

#include <memory>
#include <string>
#include <utility>

#include "simpl/ast.hh"
#include "simpl/config.hh"
#include "simpl/error.hh"

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

ExprList Parser::Parse() {
  ExprList exprs;
  while (!AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  return exprs;
}

Expr Parser::ParseSimpleExpr() {
  auto token = Advance();
  switch (token.type) {
    case Token::Type::kInteger:
      return Expr{std::get<int_type>(token.literal)};
      break;
    case Token::Type::kString:
      return Expr{std::get<std::string>(token.literal)};
      break;
    case Token::Type::kFloat:
      return Expr{std::get<float_type>(token.literal)};
      break;
    case Token::Type::kFalse:
      return Expr{false};
      break;
    case Token::Type::kTrue:
      return Expr{true};
      break;
    case Token::Type::kQuote:
      return Expr{Quoted(ParseExpr())};
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
    case Token::Type::kSymbol:
      return Expr{Symbol{token.lexeme}};
      break;
    case Token::Type::kKeyword:
      return Expr{Keyword{std::get<std::string>(token.literal)}};
      break;
    case Token::Type::kNil:
      return Expr{nullptr};
      break;
    default:
      throw Error(token, "Unexpected token");
  }
}

Expr Parser::ParseExpr() {
  if (Match(Token::Type::kLeftParen)) {
    return ParseList();
  } else if (Match(Token::Type::kLeftBracket)) {
    return ParseVector();
  } else if (Match(Token::Type::kLeftBrace)) {
    return ParseMap();
  } else {
    return ParseSimpleExpr();
  }
}

Expr Parser::ParseVector() {
  Vector args;
  while (!Check(Token::Type::kRightBracket) && !AtEnd()) {
    args.push_back(ParseExpr());
  }
  Consume(Token::Type::kRightBracket, "Expect ']' at the end of vector.");
  return Expr{std::move(args)};
}

Expr Parser::ParseList() {
  List exprs;
  while (!Check(Token::Type::kRightParen) && !AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  Consume(Token::Type::kRightParen, "Expect ')' after list.");
  return Expr{std::move(exprs)};
}

Expr Parser::ParseMap() {
  Map s_map;
  while (!Check(Token::Type::kRightBrace) && !AtEnd()) {
    auto key = ParseExpr();
    auto value = ParseExpr();
    s_map.emplace(std::move(key), std::move(value));
  }
  Consume(Token::Type::kRightBrace, "Expect '}' at the end of a map.");
  return Expr{std::move(s_map)};
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
