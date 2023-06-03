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

expr_ptr_t Parser::ParseAtom() {
  auto atom = Advance();
  switch (atom.type) {
    case Token::Type::kInteger:
      return std::make_unique<Atom>(std::get<int_type>(atom.literal));
      break;
    case Token::Type::kString:
      return std::make_unique<Atom>(std::get<std::string>(atom.literal));
      break;
    case Token::Type::kFloat:
      return std::make_unique<Atom>(std::get<float_type>(atom.literal));
      break;
    case Token::Type::kFalse:
      return std::make_unique<Atom>(false);
      break;
    case Token::Type::kTrue:
      return std::make_unique<Atom>(true);
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
    case Token::Type::kSymbol:
      return std::make_unique<Atom>(Symbol{atom.lexeme});
      break;
    case Token::Type::kNil:
      return std::make_unique<Atom>(nullptr);
      break;
    default:
      throw Error(atom, "Unexpected token");
  }
}

expr_ptr_t Parser::ParseExpr() {
  if (Match(Token::Type::kLeftParen)) {
    return ParseList();
  } else if (Match(Token::Type::kLeftBracket)) {
    return ParseVector();
  } else {
    return ParseAtom();
  }
}

expr_ptr_t Parser::ParseVector() {
  Vector::vector_impl_t args;
  while (!Check(Token::Type::kRightBracket) && !AtEnd()) {
    args.push_back(ParseExpr());
  }
  Consume(Token::Type::kRightBracket, "Expect ']' at the end of vector.");
  return std::make_unique<Vector>(std::move(args));
}

expr_list_t Parser::ParseExprs() {
  expr_list_t exprs;
  while (!Check(Token::Type::kRightParen) && !AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  return exprs;
}

expr_ptr_t Parser::ParseList() {
  expr_list_t exprs;
  while (!Check(Token::Type::kRightParen) && !AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  Consume(Token::Type::kRightParen, "Expect ')' after list.");
  return std::make_unique<List>(std::move(exprs));
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
