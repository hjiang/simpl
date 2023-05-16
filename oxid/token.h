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
    EQUAL,

    // One or two character tokens.
    BANG,
    BANG_EQUAL,
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
    DEF,
    DEFCLASS,
    DEFN,
    FALSE,
    FN,
    FOR,
    IF,
    LET,
    NIL,
    OR,
    PRINT,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    END_OF_FILE
  };

  using literal_t = std::variant<long, double, std::string, nullptr_t>;
  const Type type;
  const std::string lexeme;
  const literal_t literal;
  const int line;

  Token(Type type, const std::string &lexeme, literal_t literal, int line)
      : type(type), lexeme(lexeme), literal(literal), line(line) {}

  operator std::string() { return lexeme; }
};

#endif // TOKEN_H_

// Local Variables:
// compile-command : "bazel build //oxid:lexer"
// End:
