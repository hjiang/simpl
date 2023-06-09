// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/parser.h"

#include <gtest/gtest.h>

#include <memory>
#include <variant>

#include "simpl/ast.h"
#include "simpl/config.h"
#include "simpl/lexer.h"

namespace simpl {

TEST(Parser, Integer) {
  Lexer lexer("1");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kInteger);
  EXPECT_EQ(std::get<int_type>(tokens.front().literal), 1);
  Parser parser(tokens);
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr const *>(exprs.front().get());
  EXPECT_EQ(std::get<int_type>(*atom), 1);
}

TEST(Parser, String) {
  Lexer lexer("\"hello\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kString);
  Parser parser(tokens);
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr const *>(exprs.front().get());
  EXPECT_EQ(std::get<std::string>(*atom), "hello");
}

TEST(Parser, Bool) {
  Lexer lexer("false");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kFalse);
  Parser parser(tokens);
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr const *>(exprs.front().get());
  EXPECT_EQ(std::get<bool>(*atom), false);
}

TEST(Parser, Symbol) {
  Lexer lexer("foo");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  Parser parser(tokens);
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr const *>(exprs.front().get());
  EXPECT_EQ(std::get<Symbol>(*atom).name, "foo");
}

TEST(Parser, List) {
  Lexer lexer("(+ 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_TRUE(nullptr != exprs.front());
  auto list = std::get<std::shared_ptr<List>>(*exprs.front());
  EXPECT_TRUE(nullptr != list);
  EXPECT_EQ(list->exprs().size(), 3);
  auto i = list->exprs().begin();
  auto atom = dynamic_cast<Expr const *>(i->get());
  EXPECT_EQ(std::get<Symbol>(*atom).name, "+");
  i++;
  atom = dynamic_cast<Expr const *>(i->get());
  EXPECT_EQ(std::get<int_type>(*atom), 1);
  i++;
  atom = dynamic_cast<Expr const *>(i->get());
  EXPECT_EQ(std::get<int_type>(*atom), 2);
}

TEST(Parser, Def) {
  Lexer lexer("(def foo 42)");
  auto tokens = lexer.scan();
  EXPECT_EQ(6, tokens.size());
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
}

TEST(Parser, Vector) {
  Lexer lexer("[1 2 3]");
  auto tokens = lexer.scan();
  EXPECT_EQ(6, tokens.size());
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
  auto vec = std::get<std::shared_ptr<Vector>>(*exprs.front());
  EXPECT_TRUE(nullptr != vec);
  EXPECT_EQ(3, vec->exprs().size());
  EXPECT_EQ(1, std::get<int_type>(vec->Head()));
}

TEST(Parser, Let) {
  Lexer lexer("(let [a 2] a)");
  auto tokens = lexer.scan();
  EXPECT_EQ(9, tokens.size());
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
}

TEST(Parser, If) {
  Lexer lexer("(if a 2 3)");
  auto tokens = lexer.scan();
  EXPECT_EQ(7, tokens.size());
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
}

TEST(Parser, Nil) {
  Lexer lexer("nil");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
  auto atom = dynamic_cast<Expr const *>(exprs.front().get());
  EXPECT_EQ(std::get<std::nullptr_t>(*atom), nullptr);
}

TEST(Parser, Fn) {
  Lexer lexer("(fn [a b c] (+ a b c))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
}

}  // namespace simpl
// Local Variables:
// compile-command : "bazel test //simpl:parser_test"
// End:
