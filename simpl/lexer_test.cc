// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/lexer.hh"

#include <gtest/gtest.h>
#include <string>

#include "simpl/error.hh"

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
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, Keyword) {
  simpl::ClearError();
  Lexer lexer(":name");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kKeyword);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, EmptyKeyword) {
  simpl::ClearError();
  Lexer lexer(":");
  auto tokens = lexer.scan();
  EXPECT_TRUE(simpl::HadError());
}

TEST(Lexer, ShortestKeyword) {
  simpl::ClearError();
  Lexer lexer(":a");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kKeyword);
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

TEST(Lexer, InvalidToken) {
  simpl::ClearError();
  Lexer lexer("12p");
  lexer.scan();
  EXPECT_TRUE(simpl::HadError());
}

TEST(Lexer, Def) {
  simpl::ClearError();
  Lexer lexer("def");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
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
  simpl::ClearError();
  Lexer lexer("(-> a (+ 1 2))");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 10);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, LongArrow) {
  Lexer lexer("-->");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
}

TEST(Lexer, QuestionMark) {
  Lexer lexer("empty?");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  EXPECT_EQ(tokens.front().lexeme, "empty?");
}

TEST(Lexer, SymbolCanHaveExclamation) {
  Lexer lexer("put!");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  EXPECT_EQ(tokens.front().lexeme, "put!");
}

TEST(Lexer, QuotedList) {
  Lexer lexer("'(1 2)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kQuote);
  EXPECT_EQ(tokens.size(), 6);
}

TEST(Lexer, Map) {
  simpl::ClearError();
  Lexer lexer("{\"a\" 2}");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 5);
  EXPECT_FALSE(simpl::HadError());
}

TEST(Lexer, VariadicFn) {
  simpl::ClearError();
  Lexer lexer("(defn foo [a & b] a)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 11);
  EXPECT_FALSE(simpl::HadError());
}

}  // namespace simpl
