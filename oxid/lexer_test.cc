#include "lexer.h"

#include <gtest/gtest.h>

using namespace oxid;

TEST(Lexer, SingleChar) {
  Lexer lexer("(");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::LEFT_PAREN);
  EXPECT_EQ(tokens.back().type, Token::END_OF_FILE);
}

TEST(Lexer, LineComment) {
  Lexer lexer("; abc \n");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 1);
  EXPECT_EQ(tokens.front().type, Token::END_OF_FILE);
}
/*
TEST(Lexer, SimpleExpression) {
  Lexer lexer("(+ 1 2)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
}
  */

// Local Variables:
// compile-command : "bazel test //oxid:lexer_test"
// End:
