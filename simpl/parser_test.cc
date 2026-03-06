// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/parser.hh"

#include <gtest/gtest.h>

#include <string>
#include <variant>

#include "simpl/ast.hh"
#include "simpl/config.hh"
#include "simpl/lexer.hh"

namespace simpl {

static ExprList Parse(const std::string& source) {
  Lexer lexer(source);
  return Parser(lexer.scan()).Parse();
}

TEST(Parser, Integer) {
  Lexer lexer("1");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kInteger);
  EXPECT_EQ(std::get<int_type>(tokens.front().literal), 1);
  auto exprs = Parse("1");
  EXPECT_EQ(std::get<int_type>(exprs.front()), 1);
}

TEST(Parser, String) {
  Lexer lexer("\"hello\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kString);
  EXPECT_EQ(std::get<std::string>(Parse("\"hello\"").front()), "hello");
}

TEST(Parser, Bool) {
  Lexer lexer("false");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kFalse);
  EXPECT_EQ(std::get<bool>(Parse("false").front()), false);
}

TEST(Parser, Symbol) {
  Lexer lexer("foo");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  EXPECT_EQ(std::get<Symbol>(Parse("foo").front()).name, "foo");
}

TEST(Parser, Keyword) {
  Lexer lexer(":foo");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kKeyword);
  EXPECT_EQ(std::get<Keyword>(Parse(":foo").front()).name, "foo");
}

TEST(Parser, List) {
  auto exprs = Parse("(+ 1 2)");
  auto list = std::get<List>(exprs.front());
  EXPECT_EQ(list.size(), 3);
  auto i = list.begin();
  EXPECT_EQ(std::get<Symbol>(*i).name, "+");
  i++;
  EXPECT_EQ(std::get<int_type>(*i), 1);
  i++;
  EXPECT_EQ(std::get<int_type>(*i), 2);
}

TEST(Parser, Def) {
  Lexer lexer("(def foo 42)");
  auto tokens = lexer.scan();
  EXPECT_EQ(6, tokens.size());
  EXPECT_EQ(1, Parse("(def foo 42)").size());
}

TEST(Parser, Vector) {
  Lexer lexer("[1 2 3]");
  auto tokens = lexer.scan();
  EXPECT_EQ(6, tokens.size());
  auto exprs = Parse("[1 2 3]");
  EXPECT_EQ(1, exprs.size());
  auto vec = std::get<Vector>(exprs.front());
  EXPECT_EQ(3, vec.size());
  EXPECT_EQ(1, std::get<int_type>(vec.front()));
}

TEST(Parser, Map) {
  auto exprs = Parse("{:a 1 :b 2}");
  auto m = std::get<Map>(exprs.front());
  EXPECT_EQ(1, exprs.size());
  EXPECT_EQ(2, m.size());
  EXPECT_EQ(Expr{1}, m[Expr{Keyword{"a"}}]);
  EXPECT_EQ(Expr{2}, m[Expr{Keyword{"b"}}]);
}

TEST(Parser, Let) {
  Lexer lexer("(let [a 2] a)");
  auto tokens = lexer.scan();
  EXPECT_EQ(9, tokens.size());
  EXPECT_EQ(1, Parse("(let [a 2] a)").size());
}

TEST(Parser, If) {
  Lexer lexer("(if a 2 3)");
  auto tokens = lexer.scan();
  EXPECT_EQ(7, tokens.size());
  EXPECT_EQ(1, Parse("(if a 2 3)").size());
}

TEST(Parser, Nil) {
  auto exprs = Parse("nil");
  EXPECT_EQ(1, exprs.size());
  EXPECT_EQ(std::get<std::nullptr_t>(exprs.front()), nullptr);
}

TEST(Parser, Fn) { EXPECT_EQ(1, Parse("(fn [a b c] (+ a b c))").size()); }

TEST(Parser, SyntaxQuote) {
  auto exprs = Parse("`(a b)");
  auto qt = std::get<Quoted>(exprs.front());
  EXPECT_EQ(qt.kind(), Quoted::Kind::kSyntaxQuote);
  EXPECT_TRUE(std::holds_alternative<List>(qt.expr()));
}

TEST(Parser, Unquote) {
  auto exprs = Parse("~x");
  auto qt = std::get<Quoted>(exprs.front());
  EXPECT_EQ(qt.kind(), Quoted::Kind::kUnquote);
  EXPECT_EQ(std::get<Symbol>(qt.expr()).name, "x");
}

TEST(Parser, SpliceUnquote) {
  auto exprs = Parse("~@body");
  auto qt = std::get<Quoted>(exprs.front());
  EXPECT_EQ(qt.kind(), Quoted::Kind::kSplice);
  EXPECT_EQ(std::get<Symbol>(qt.expr()).name, "body");
}

TEST(Parser, SyntaxQuoteWithUnquote) {
  auto exprs = Parse("`(if ~cond (do ~@body) nil)");
  auto qt = std::get<Quoted>(exprs.front());
  EXPECT_EQ(qt.kind(), Quoted::Kind::kSyntaxQuote);
  auto list = std::get<List>(qt.expr());
  EXPECT_EQ(list.size(), 4);
}

}  // namespace simpl
// Local Variables:
// compile-command : "bazel test //simpl:parser_test"
// End:
