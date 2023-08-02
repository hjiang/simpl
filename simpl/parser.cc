// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/parser.h"

#include <memory>
#include <string>

#include "simpl/ast.h"
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
  return Expr{args};
}

Expr Parser::ParseList() {
  expr_list_t exprs;
  while (!Check(Token::Type::kRightParen) && !AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  Consume(Token::Type::kRightParen, "Expect ')' after list.");
  return Expr{List(std::move(exprs))};
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
