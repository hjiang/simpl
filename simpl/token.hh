// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_TOKEN_H_
#define SIMPL_TOKEN_H_

#include <string>
#include <variant>

#include "simpl/config.hh"

namespace simpl {

class Token {
 public:
  enum Type {
    // Single-character tokens.
    kLeftParen,
    kRightParen,
    kLeftBracket,
    kRightBracket,
    kLeftBrace,
    kRightBrace,
    kMinus,
    kPlus,
    kSlash,
    kStar,
    kEqual,
    kPercent,

    // One or two character tokens.
    kBang,
    kBangEqual,
    kGreater,
    kGreaterEqual,
    kLess,
    kLessEqual,
    kQuote,

    // Literals.
    kSymbol,
    kKeyword,
    kString,
    kInteger,
    kFloat,
    kNil,

    // Keywords.
    kFalse,
    kTrue,

    kEof
  };

  using literal_t =
      std::variant<int_type, float_type, std::string, std::nullptr_t>;
  const Type type;
  const std::string lexeme;
  const literal_t literal;
  const size_t line;

  Token(Type type, const std::string &lexeme, literal_t literal, size_t line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}

  operator std::string() { return lexeme; }
};

}  // namespace simpl

#endif  // SIMPL_TOKEN_H_

// Local Variables:
// compile-command : "bazel build //simpl:lexer"
// End:
