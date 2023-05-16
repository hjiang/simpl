#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <variant>

class Token {
public:
  enum Type {
    // Single-character tokens.
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    INTEGER,
    FLOAT,

    // Keywords.
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    END_OF_FILE
  };

  using literal_t = std::variant<long, double, std::string>;
  const Type type;
  const std::string lexeme;
  const literal_t literal;
  const int line;

  Token(Type type, const std::string &lexeme, literal_t literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}

  operator std::string() { return lexeme; }
};

#endif // TOKEN_H_
