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

std::shared_ptr<Expr> Parser::Parse() { return ParseAtom(); }

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
    case Token::Type::kSymbol:
      return std::make_shared<Expr::Atom>(Symbol{atom.lexeme});
    case Token::Type::kNil:
      return std::make_shared<Expr::Atom>(nullptr);
    default:
      return nullptr;  // FIXME: raise error
  }
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
