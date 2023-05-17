#include "lexer.h"

#include <cassert>
#include <cctype>
#include <list>
#include <string>
#include <unordered_map>

#include "error.h"
#include "token.h"

namespace oxid {

static const std::unordered_map<std::string, Token::Type> kKeywords{
    {"and", Token::kAnd},     {"class", Token::kClass},
    {"def", Token::kDef},     {"defclass", Token::kDefClass},
    {"defn", Token::kDefn},   {"false", Token::kFalse},
    {"for", Token::kFor},     {"fn", Token::kFn},
    {"if", Token::kIf},       {"let", Token::kLet},
    {"nil", Token::kNil},     {"or", Token::kOr},
    {"print", Token::kPrint}, {"super", Token::kSuper},
    {"this", Token::kThis},   {"true", Token::kTrue},
    {"var", Token::kVar},     {"while", Token::kWhile}};

static bool IsAlphaOrUnderscore(char c) { return isalpha(c) || c == '_'; }
static bool IsAlphaNumOrUnderscore(char c) { return isalnum(c) || c == '_'; }

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
    case '-':
      if (isspace(Peek())) {
        AddToken(Token::kMinus);
      } else {
        Number();
      }
      break;
    case '+':
      if (isspace(Peek())) {
        AddToken(Token::kPlus);
      } else {
        Number();
      }
      break;
    case ';':
      while (Peek() != '\n' && !AtEnd()) Advance();
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
    default:
      if (isdigit(c)) {
        Number();
      } else if (IsAlphaOrUnderscore(c)) {
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
};

void Lexer::String() {
  while (Peek() != '"' && !AtEnd()) {
    if (Peek() == '\n') line_++;
    Advance();
  }

  if (AtEnd()) {
    Error(line_, "Unterminated string.");
    return;
  }

  char c = Advance();
  assert('"' == c);
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
  if (is_float) {
    AddToken(Token::kFloat,
             std::stod(source_.substr(start_, current_ - start_)));
  } else {
    AddToken(Token::kInteger,
             std::stoi(source_.substr(start_, current_ - start_)));
  }
}

void Lexer::Symbol() {
  while (IsAlphaNumOrUnderscore(Peek())) Advance();

  const auto text = source_.substr(start_, current_ - start_);
  const auto it = kKeywords.find(text);
  it != kKeywords.end() ? AddToken(it->second) : AddToken(Token::kSymbol);
}

char Lexer::PeekNext() {
  if (current_ + 1 >= source_.size()) return '\0';
  return source_[current_ + 1];
}

}  // namespace oxid

// Local Variables:
// compile-command : "bazel test //oxid:lexer_test"
// End:
