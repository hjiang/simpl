// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include <gtest/gtest.h>

#include <sstream>

#include "simpl/lexer.hh"
#include "simpl/parser.hh"

namespace simpl {

TEST(AST, IntegerOutput) {
  Lexer lexer("42");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  std::ostringstream oss;
  oss << expr.front();
  EXPECT_EQ(oss.str(), "42");
}

TEST(AST, ListOutput) {
  Lexer lexer("(42 true \"hello\" 3.14)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  std::ostringstream oss;
  oss << expr.front();
  EXPECT_EQ(oss.str(), "(42 true \"hello\" 3.14)");
}

TEST(AST, EmptyListOutput) {
  Lexer lexer("()");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  std::ostringstream oss;
  oss << expr.front();
  EXPECT_EQ(oss.str(), "()");
}

TEST(AST, VectorOutput) {
  Lexer lexer("[42 true good 3.14]");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  std::ostringstream oss;
  oss << expr.front();
  EXPECT_EQ(oss.str(), "[42 true good 3.14]");
}

TEST(AST, EmptyVectorOutput) {
  Lexer lexer("[]");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  std::ostringstream oss;
  oss << expr.front();
  EXPECT_EQ(oss.str(), "[]");
}

TEST(AST, QuoteOutput) {
  Lexer lexer("'(+ 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  std::ostringstream oss;
  oss << expr.front();
  EXPECT_EQ(oss.str(), "'(+ 1 2)");
}

}  // namespace simpl
