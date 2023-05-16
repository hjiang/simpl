#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <variant>

class Token {
public:
  enum Type {
    // Single-character tokens.
    kLeftParen,
    kRightParen,
    kLeftBrace,
    kRightBrace,
    kComma,
    kDot,
    kMinus,
    kPlus,
    kSlash,
    kStar,
    kEqual,

    // One or two character tokens.
    kBang,
    kBangEqual,
    kGreater,
    kGreaterEqual,
    kLess,
    kLessEqual,

    // Literals.
    kIdentifier,
    kString,
    kInteger,
    kFloat,

    // Keywords.
    kAnd,
    kClass,
    kDef,
    kDefclass,
    kDefn,
    kFalse,
    kFn,
    kFor,
    kIf,
    kLet,
    kNil,
    kOr,
    kPrint,
    kSuper,
    kThis,
    kTrue,
    kVar,
    kWhile,

    kEof
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
