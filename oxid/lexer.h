// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef OXID_LEXER_H_
#define OXID_LEXER_H_

#include <list>
#include <string>

#include "oxid/token.h"

namespace oxid {

class Lexer {
 public:
  explicit Lexer(const std::string &source);
  std::list<Token> scan();

 private:
  bool AtEnd() { return current_ >= source_.length(); }
  void ScanToken();
  char Advance() { return source_[current_++]; }
  void AddToken(Token::Type type, const Token::literal_t &literal = nullptr);
  bool Match(char expected);
  char Peek() { return AtEnd() ? '\0' : source_[current_]; }
  char PeekNext();
  void String();
  void Number();
  void Symbol();
  const std::string source_;
  std::list<Token> tokens_;
  size_t start_ = 0;
  size_t current_ = 0;
  size_t line_ = 1;
};

}  // namespace oxid

#endif  // OXID_LEXER_H_

// Local Variables:
// compile-command : "bazel test //oxid:lexer_test"
// End:
