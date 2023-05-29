// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/lexer.h"

#include <gtest/gtest.h>

#include "simpl/error.h"

namespace simpl {

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
  simpl::ClearError();
  Lexer lexer("(   )");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Bracket) {
  simpl::ClearError();
  Lexer lexer("[]");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, String) {
  simpl::ClearError();
  Lexer lexer("\"abc\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<std::string>(tokens.front().literal), "abc");
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, EmptyString) {
  simpl::ClearError();
  Lexer lexer("\"\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<std::string>(tokens.front().literal), "");
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Float) {
  simpl::ClearError();
  Lexer lexer("3.14");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_DOUBLE_EQ(std::get<float_type>(tokens.front().literal), 3.14);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Integer) {
  simpl::ClearError();
  Lexer lexer("256");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<int_type>(tokens.front().literal), 256);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, SimpleExpression) {
  simpl::ClearError();
  Lexer lexer("(+ 1 2)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 6);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Identifier) {
  simpl::ClearError();
  Lexer lexer("if foobar");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
  EXPECT_EQ(tokens.front().type, Token::kIf);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, NegativeInteger) {
  simpl::ClearError();
  Lexer lexer("-123");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<int_type>(tokens.front().literal), -123);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Def) {
  simpl::ClearError();
  Lexer lexer("def");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kDef);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Symbol) {
  simpl::ClearError();
  Lexer lexer("asdf");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  EXPECT_EQ(tokens.front().lexeme, "asdf");
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Multiply) {
  Lexer lexer("(* 3 2)");
  auto tokens = lexer.scan();
  auto it = tokens.begin();
  EXPECT_EQ((it++)->type, Token::kLeftParen);
  EXPECT_EQ((it++)->type, Token::kStar);
  EXPECT_EQ((it++)->type, Token::kInteger);
  EXPECT_EQ((it++)->type, Token::kInteger);
  EXPECT_EQ((it++)->type, Token::kRightParen);
  EXPECT_EQ((it++)->type, Token::kEof);
  EXPECT_EQ(tokens.size(), 6);
}

TEST(Lexer, Nil) {
  Lexer lexer("nil");
  auto tokens = lexer.scan();
  auto it = tokens.begin();
  EXPECT_EQ((it++)->type, Token::kNil);
  EXPECT_EQ((it++)->type, Token::kEof);
  EXPECT_EQ(tokens.size(), 2);
}

TEST(Lexer, Equal) {
  Lexer lexer("=");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kEqual);
}

TEST(Lexer, Arrow) {
  Lexer lexer("-->");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
}

TEST(Lexer, SymbolCanHaveExclamation) {
  Lexer lexer("put!");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
}

TEST(Lexer, QuotedList) {
  Lexer lexer("'(1 2)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kQuote);
  EXPECT_EQ(tokens.size(), 6);
}

}  // namespace simpl

// Local Variables:
// compile-command : "bazel test //simpl:lexer_test"
// End:
