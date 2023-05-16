#include "error.h"
#include "lexer.h"

#include <gtest/gtest.h>

using namespace oxid;

TEST(Lexer, SingleChar) {
  Lexer lexer("(");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kLeftParen);
  EXPECT_EQ(tokens.back().type, Token::kEof);
}

TEST(Lexer, LineComment) {
  Lexer lexer("; abc \n");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 1);
  EXPECT_EQ(tokens.front().type, Token::kEof);
}

TEST(Lexer, WhiteSpace) {
  oxid::ClearError();
  Lexer lexer("(   )");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
  EXPECT_FALSE(oxid::HadError());
}

TEST(Lexer, Bracket) {
  oxid::ClearError();
  Lexer lexer("[]");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
  EXPECT_FALSE(oxid::HadError());
}

TEST(Lexer, String) {
  oxid::ClearError();
  Lexer lexer("\"abc\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<std::string>(tokens.front().literal), "abc");
  EXPECT_FALSE(oxid::HadError());
}

TEST(Lexer, EmptyString) {
  oxid::ClearError();
  Lexer lexer("\"\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<std::string>(tokens.front().literal), "");
  EXPECT_FALSE(oxid::HadError());
}
// Local Variables:
// compile-command : "bazel test //oxid:lexer_test"
// End:
