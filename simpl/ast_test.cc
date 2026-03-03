// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "simpl/lexer.hh"
#include "simpl/parser.hh"

namespace simpl {

static std::string FormatExpr(const std::string& source) {
  Lexer lexer(source);
  Parser parser(lexer.scan());
  std::ostringstream oss;
  oss << parser.Parse().front();
  return oss.str();
}

TEST(AST, IntegerOutput) { EXPECT_EQ(FormatExpr("42"), "42"); }

TEST(AST, ListOutput) {
  EXPECT_EQ(FormatExpr("(42 true \"hello\" 3.14)"), "(42 true \"hello\" 3.14)");
}

TEST(AST, EmptyListOutput) { EXPECT_EQ(FormatExpr("()"), "()"); }

TEST(AST, VectorOutput) {
  EXPECT_EQ(FormatExpr("[42 true good 3.14]"), "[42 true good 3.14]");
}

TEST(AST, EmptyVectorOutput) { EXPECT_EQ(FormatExpr("[]"), "[]"); }

TEST(AST, QuoteOutput) { EXPECT_EQ(FormatExpr("'(+ 1 2)"), "'(+ 1 2)"); }

}  // namespace simpl
