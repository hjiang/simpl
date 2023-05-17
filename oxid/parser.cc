#include "parser.h"

#include <memory>

#include "error.h"

namespace oxid {

static void Error(const Token& token, const std::string& message) {
  if (token.type == Token::Type::kEof) {
    Report(token.line, " at end", message);
  } else {
    Report(token.line, " at '" + token.lexeme + "'", message);
  }
}

Parser::ParseError Parser::Error(const Token& token,
                                 const std::string& message) {
  oxid::Error(token, message);
  return ParseError(message);
}

const Token& Parser::Consume(Token::Type type, const std::string& message) {
  if (Check(type)) return Advance();
  throw Error(Peek(), message);
}

void Expr::Atom::Accept(ExprVisitor* visitor) const { visitor->Visit(*this); }

void Expr::List::Accept(ExprVisitor* visitor) const { visitor->Visit(*this); }

std::shared_ptr<Expr> Parser::Parse() {
  try {
    return ParseExpr();
  } catch (const ParseError& e) {
    return nullptr;
  }
}

std::shared_ptr<Expr> Parser::ParseAtom() {
  auto atom = Advance();
  switch (atom.type) {
    case Token::Type::kInteger:
      return std::make_shared<Expr::Atom>(std::get<long>(atom.literal));
      break;
    case Token::Type::kString:
      return std::make_shared<Expr::Atom>(std::get<std::string>(atom.literal));
      break;
    case Token::Type::kFloat:
      return std::make_shared<Expr::Atom>(std::get<double>(atom.literal));
      break;
    case Token::Type::kFalse:
      return std::make_shared<Expr::Atom>(false);
      break;
    case Token::Type::kTrue:
      return std::make_shared<Expr::Atom>(true);
      break;
    case Token::Type::kMinus:
    case Token::Type::kPlus:
    case Token::Type::kSlash:
    case Token::Type::kStar:
    case Token::Type::kEqual:
    case Token::Type::kBang:
    case Token::Type::kBangEqual:
    case Token::Type::kGreater:
    case Token::Type::kGreaterEqual:
    case Token::Type::kLess:
    case Token::Type::kLessEqual:
    case Token::Type::kAnd:
    case Token::Type::kDef:
    case Token::Type::kDefn:
    case Token::Type::kFn:
    case Token::Type::kFor:
    case Token::Type::kIf:
    case Token::Type::kLet:
    case Token::Type::kOr:
    case Token::Type::kPrint:
    case Token::Type::kVar:
    case Token::Type::kWhile:
    case Token::Type::kSymbol:
      return std::make_shared<Expr::Atom>(Symbol{atom.lexeme});
    case Token::Type::kNil:
      return std::make_shared<Expr::Atom>(nullptr);
    default:
      throw Error(atom, "Unexpected token");
  }
}

std::shared_ptr<Expr> Parser::ParseExpr() {
  if (Peek().type == Token::Type::kLeftParen) {
    return ParseList();
  } else {
    return ParseAtom();
  }
}

std::shared_ptr<Expr> Parser::ParseList() {
  Consume(Token::Type::kLeftParen, "Expect '(' before list.");
  std::list<std::shared_ptr<Expr>> exprs;
  while (!Check(Token::Type::kRightParen) && !AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  Consume(Token::Type::kRightParen, "Expect ')' after list.");
  return std::make_shared<Expr::List>(exprs);
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
  if (!AtEnd()) current_++;
  return Previous();
}

}  // namespace oxid
