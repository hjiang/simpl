#include "parser.h"

#include <gtest/gtest.h>

#include <memory>

#include "lexer.h"

using namespace oxid;

TEST(Parser, Integer) {
  Lexer lexer("1");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kInteger);
  EXPECT_EQ(std::get<long>(tokens.front().literal), 1);
  Parser parser(tokens);
  auto atom = std::dynamic_pointer_cast<Expr::Atom>(parser.Parse());
  EXPECT_EQ(atom->value<long>(), 1);
}

TEST(Parser, String) {
  Lexer lexer("\"hello\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kString);
  Parser parser(tokens);
  auto atom = std::dynamic_pointer_cast<Expr::Atom>(parser.Parse());
  EXPECT_EQ(atom->value<std::string>(), "hello");
}

TEST(Parser, Symbol) {
  Lexer lexer("foo");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  Parser parser(tokens);
  auto atom = std::dynamic_pointer_cast<Expr::Atom>(parser.Parse());
  EXPECT_EQ(atom->value<Symbol>().name, "foo");
}

// Local Variables:
// compile-command : "bazel test //oxid:parser_test"
// End:
