// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/lexer.hh"

#include <gtest/gtest.h>
#include <string>

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
  Lexer lexer("(   )");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
}

TEST(Lexer, Bracket) {
  Lexer lexer("[]");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
}

TEST(Lexer, String) {
  Lexer lexer("\"abc\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<std::string>(tokens.front().literal), "abc");
}

TEST(Lexer, EmptyString) {
  Lexer lexer("\"\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<std::string>(tokens.front().literal), "");
}

TEST(Lexer, Float) {
  Lexer lexer("3.14");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_DOUBLE_EQ(std::get<float_type>(tokens.front().literal), 3.14);
}

TEST(Lexer, Integer) {
  Lexer lexer("256");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<int_type>(tokens.front().literal), 256);
}

TEST(Lexer, SimpleExpression) {
  Lexer lexer("(+ 1 2)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 6);
}

TEST(Lexer, Identifier) {
  Lexer lexer("if foobar");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 3);
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
}

TEST(Lexer, Keyword) {
  Lexer lexer(":name");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kKeyword);
}

TEST(Lexer, EmptyKeyword) {
  Lexer lexer(":");
  EXPECT_THROW(lexer.scan(), std::runtime_error);
}

TEST(Lexer, ShortestKeyword) {
  Lexer lexer(":a");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kKeyword);
}

TEST(Lexer, NegativeInteger) {
  Lexer lexer("-123");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(std::get<int_type>(tokens.front().literal), -123);
}

TEST(Lexer, InvalidToken) {
  Lexer lexer("12p");
  EXPECT_THROW(lexer.scan(), std::runtime_error);
}

TEST(Lexer, Def) {
  Lexer lexer("def");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
}

TEST(Lexer, Symbol) {
  Lexer lexer("asdf");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 2);
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  EXPECT_EQ(tokens.front().lexeme, "asdf");
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
  Lexer lexer("(-> a (+ 1 2))");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 10);
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
  Lexer lexer("{\"a\" 2}");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 5);
}

TEST(Lexer, VariadicFn) {
  Lexer lexer("(defn foo [a & b] a)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.size(), 11);
}

TEST(Lexer, Backtick) {
  Lexer lexer("`(a b)");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kBacktick);
}

TEST(Lexer, Tilde) {
  Lexer lexer("~x");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kTilde);
}

TEST(Lexer, TildeSplice) {
  Lexer lexer("~@body");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kTildeSplice);
  auto it = tokens.begin();
  ++it;
  EXPECT_EQ(it->type, Token::kSymbol);
  EXPECT_EQ(it->lexeme, "body");
}

TEST(Lexer, SyntaxQuoteExpression) {
  Lexer lexer("`(if ~cond (do ~@body) nil)");
  auto tokens = lexer.scan();
  auto it = tokens.begin();
  EXPECT_EQ((it++)->type, Token::kBacktick);
  EXPECT_EQ((it++)->type, Token::kLeftParen);
  EXPECT_EQ((it++)->type, Token::kSymbol);   // if
  EXPECT_EQ((it++)->type, Token::kTilde);     // ~
  EXPECT_EQ((it++)->type, Token::kSymbol);    // cond
  EXPECT_EQ((it++)->type, Token::kLeftParen);
  EXPECT_EQ((it++)->type, Token::kSymbol);    // do
  EXPECT_EQ((it++)->type, Token::kTildeSplice);  // ~@
  EXPECT_EQ((it++)->type, Token::kSymbol);    // body
  EXPECT_EQ((it++)->type, Token::kRightParen);
  EXPECT_EQ((it++)->type, Token::kNil);
  EXPECT_EQ((it++)->type, Token::kRightParen);
  EXPECT_EQ((it++)->type, Token::kEof);
}

}  // namespace simpl
