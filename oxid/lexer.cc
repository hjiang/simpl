#include "lexer.h"

#include <list>

#include "error.h"
#include "token.h"

namespace oxid {

Lexer::Lexer(const std::string &source) : source_(source), tokens_() {}

std::list<Token> Lexer::scan() {
  while (!AtEnd()) {
    // We are at the beginning of the next lexeme.
    start_ = current_;
    ScanToken();
  }

  tokens_.push_back(Token(Token::kEof, "", nullptr, line_));
  return tokens_;
}

void Lexer::ScanToken() {
  char c = Advance();
  switch (c) {
  case '(':
    AddToken(Token::kLeftParen);
    break;
  case ')':
    AddToken(Token::kRightParen);
    break;
  case '{':
    AddToken(Token::kLeftBrace);
    break;
  case '}':
    AddToken(Token::kRightBrace);
    break;
  case ',':
    AddToken(Token::kComma);
    break;
  case '.':
    AddToken(Token::kDot);
    break;
  case '-':
    AddToken(Token::kMinus);
    break;
  case '+':
    AddToken(Token::kPlus);
    break;
  case ';':
    while (Peek() != '\n' && !AtEnd())
      Advance();
    break;
  case '*':
    AddToken(Token::kStar);
  case '!':
    AddToken(Match('=') ? Token::kBangEqual : Token::kBang);
    break;
  case '<':
    AddToken(Match('=') ? Token::kLessEqual : Token::kLess);
    break;
  case '>':
    AddToken(Match('=') ? Token::kGreaterEqual : Token::kGreater);
    break;
  default:
    Error(line_, "Unexpected character.");
    break;
  }
}

bool Lexer::Match(char expected) {
  if (AtEnd())
    return false;
  if (source_[current_] != expected)
    return false;
  current_++;
  return true;
}

void Lexer::AddToken(Token::Type type, const Token::literal_t &literal) {
  tokens_.push_back(
      Token(type, source_.substr(start_, current_ - start_), literal, line_));
};

} // namespace oxid

// Local Variables:
// compile-command : "bazel build //oxid:lexer"
// End:
