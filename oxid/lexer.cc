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

  tokens_.push_back(Token(Token::END_OF_FILE, "", nullptr, line_));
  return tokens_;
}

void Lexer::ScanToken() {
  char c = Advance();
  switch (c) {
  case '(':
    AddToken(Token::LEFT_PAREN);
    break;
  case ')':
    AddToken(Token::RIGHT_PAREN);
    break;
  case '{':
    AddToken(Token::LEFT_BRACE);
    break;
  case '}':
    AddToken(Token::RIGHT_BRACE);
    break;
  case ',':
    AddToken(Token::COMMA);
    break;
  case '.':
    AddToken(Token::DOT);
    break;
  case '-':
    AddToken(Token::MINUS);
    break;
  case '+':
    AddToken(Token::PLUS);
    break;
  case ';':
    AddToken(Token::SEMICOLON);
    break;
  case '*':
    AddToken(Token::STAR);
  case '!':
    AddToken(Match('=') ? Token::BANG_EQUAL : Token::BANG);
    break;
  case '<':
    AddToken(Match('=') ? Token::LESS_EQUAL : Token::LESS);
    break;
  case '>':
    AddToken(Match('=') ? Token::GREATER_EQUAL : Token::GREATER);
    break;
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
