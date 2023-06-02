// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/lexer.h"

#include <cassert>
#include <cctype>
#include <list>
#include <string>
#include <unordered_map>

#include "simpl/error.h"
#include "simpl/token.h"

namespace simpl {

static const std::unordered_map<std::string, Token::Type> kKeywords{
    {"false", Token::kFalse},
    {"let", Token::kLet},
    {"nil", Token::kNil},
    {"true", Token::kTrue},
};

static bool CanStartSymbol(char c) {
  return isalpha(c) || '_' == c || '-' == c;
}
static bool CanBeInSymbol(char c) {
  return isalnum(c) || '_' == c || '-' == c || '!' == c || '/' == c;
}

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
    case '[':
      AddToken(Token::kLeftBracket);
      break;
    case ']':
      AddToken(Token::kRightBracket);
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
    case ';':
      while (Peek() != '\n' && !AtEnd()) Advance();
      break;
    case '*':
      AddToken(Token::kStar);
      break;
    case '/':
      AddToken(Token::kSlash);
      break;
    case '%':
      AddToken(Token::kPercent);
      break;
    case '!':
      AddToken(Match('=') ? Token::kBangEqual : Token::kBang);
      break;
    case '<':
      AddToken(Match('=') ? Token::kLessEqual : Token::kLess);
      break;
    case '>':
      AddToken(Match('=') ? Token::kGreaterEqual : Token::kGreater);
      break;
    case '=':
      AddToken(Token::kEqual);
      break;
    case '\'':
      AddToken(Token::kQuote);
      break;
    case '"':
      String();
      break;
    case ' ':
    case '\r':
    case '\t':
      break;
    case '\n':
      line_++;
      break;
    case '+':
      if (isspace(Peek())) {
        AddToken(Token::kPlus);
      } else {
        Number();
      }
      // Perhaps we will allow + to start a symbol, but let's be strict for now.
      break;
    case '-':
      if (isspace(Peek())) {
        AddToken(Token::kMinus);
        break;
      } else if (isdigit(Peek())) {
        Number();
        break;
      }
      // fallthrough. - can start a symbol
    default:
      if (isdigit(c)) {
        Number();
      } else if (CanStartSymbol(c)) {
        Symbol();
      } else {
        Error(line_, "Unexpected character.");
      }
      break;
  }
}

bool Lexer::Match(char expected) {
  if (AtEnd()) return false;
  if (source_[current_] != expected) return false;
  current_++;
  return true;
}

void Lexer::AddToken(Token::Type type, const Token::literal_t &literal) {
  tokens_.push_back(
      Token(type, source_.substr(start_, current_ - start_), literal, line_));
}

void Lexer::String() {
  while (Peek() != '"' && !AtEnd()) {
    if (Peek() == '\n') line_++;
    Advance();
  }

  if (AtEnd()) {
    Error(line_, "Unterminated string.");
    return;
  }

  Advance();  // move past the closing '"'
  AddToken(Token::kString, source_.substr(start_ + 1, current_ - start_ - 2));
}

void Lexer::Number() {
  bool is_float = false;
  while (isdigit(Peek())) Advance();

  if (Peek() == '.' && isdigit(PeekNext())) {
    is_float = true;
    Advance();
    while (isdigit(Peek())) Advance();
  }
  try {
    if (is_float) {
      AddToken(Token::kFloat,
               std::stod(source_.substr(start_, current_ - start_)));
    } else {
      AddToken(Token::kInteger,
               std::stoi(source_.substr(start_, current_ - start_)));
    }
  } catch (const std::invalid_argument &e) {
    Error(line_, "Invalid number.");
  }
}

void Lexer::Symbol() {
  while (CanBeInSymbol(Peek())) Advance();

  const auto text = source_.substr(start_, current_ - start_);
  const auto it = kKeywords.find(text);
  it != kKeywords.end() ? AddToken(it->second) : AddToken(Token::kSymbol);
}

char Lexer::PeekNext() {
  if (current_ + 1 >= source_.size()) return '\0';
  return source_[current_ + 1];
}

}  // namespace simpl

// Local Variables:
// compile-command : "bazel test //simpl:lexer_test"
// End:
