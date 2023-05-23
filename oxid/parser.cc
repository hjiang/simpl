#include "parser.h"

#include <memory>
#include <string>

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

void Expr::Atom::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }

void Expr::List::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }

void Expr::Def::Accept(Expr::Visitor* visitor) const { visitor->Visit(*this); }

std::list<std::unique_ptr<const Expr>> Parser::Parse() {
  std::list<std::unique_ptr<const Expr>> exprs;
  while (!AtEnd()) {
    exprs.push_back(ParseExpr());
  }
  return exprs;
}

std::unique_ptr<Expr> Parser::ParseAtom() {
  auto atom = Advance();
  switch (atom.type) {
    case Token::Type::kInteger:
      return std::make_unique<Expr::Atom>(std::get<long>(atom.literal));
      break;
    case Token::Type::kString:
      return std::make_unique<Expr::Atom>(std::get<std::string>(atom.literal));
      break;
    case Token::Type::kFloat:
      return std::make_unique<Expr::Atom>(std::get<double>(atom.literal));
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
    case Token::Type::kAnd:
    case Token::Type::kOr:
    case Token::Type::kPrint:
    case Token::Type::kVar:
    case Token::Type::kWhile:
    case Token::Type::kSymbol:
      return std::make_unique<Expr::Atom>(Expr::Symbol{atom.lexeme});
    case Token::Type::kNil:
      return std::make_unique<Expr::Atom>(nullptr);
    default:
      throw Error(atom, "Unexpected token");
  }
}

std::unique_ptr<Expr> Parser::ParseExpr() {
  if (Match(Token::Type::kLeftParen)) {
    if (Match(Token::Type::kDef)) {
      return ParseDef();
    }
    return ParseList();
  } else {
    return ParseAtom();
  }
}

std::unique_ptr<Expr> Parser::ParseDef() {
  auto token = Advance();
  if (token.type != Token::Type::kSymbol) {
    throw Error(token, "Expected symbol after def");
  }
  auto name = token.lexeme;
  auto expr = ParseExpr();
  Consume(Token::Type::kRightParen, "Expect ')' after definition.");
  return std::make_unique<Expr::Def>(std::move(name), std::move(expr));
}

std::unique_ptr<Expr> Parser::ParseList() {
  std::list<std::unique_ptr<Expr>> exprs;
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

}  // namespace oxid

// Local Variables:
// compile-command : "bazel test //oxid:parser_test"
// End:
