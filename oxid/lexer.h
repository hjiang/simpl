// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef LEXER_H_
#define LEXER_H_

#include <list>
#include <string>

#include "oxid/token.h"

namespace oxid {

class Lexer {
 public:
  explicit Lexer(const std::string &source);
  std::list<Token> scan();

 private:
  bool AtEnd() { return current_ >= source_.length(); };
  void ScanToken();
  char Advance() { return source_[current_++]; };
  void AddToken(Token::Type type, const Token::literal_t &literal = nullptr);
  bool Match(char expected);
  char Peek() { return AtEnd() ? '\0' : source_[current_]; };
  char PeekNext();
  void String();
  void Number();
  void Symbol();
  const std::string source_;
  std::list<Token> tokens_;
  int start_ = 0;
  int current_ = 0;
  int line_ = 1;
};

}  // namespace oxid

#endif  // LEXER_H_

// Local Variables:
// compile-command : "bazel test //oxid:lexer_test"
// End:
