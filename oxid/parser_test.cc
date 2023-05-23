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
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr::Atom const *>(exprs.front().get());
  EXPECT_EQ(atom->value<long>(), 1);
}

TEST(Parser, String) {
  Lexer lexer("\"hello\"");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kString);
  Parser parser(tokens);
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr::Atom const *>(exprs.front().get());
  EXPECT_EQ(atom->value<std::string>(), "hello");
}

TEST(Parser, Bool) {
  Lexer lexer("false");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kFalse);
  Parser parser(tokens);
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr::Atom const *>(exprs.front().get());
  EXPECT_EQ(atom->value<bool>(), false);
}

TEST(Parser, Symbol) {
  Lexer lexer("foo");
  auto tokens = lexer.scan();
  EXPECT_EQ(tokens.front().type, Token::kSymbol);
  Parser parser(tokens);
  auto exprs = parser.Parse();
  auto atom = dynamic_cast<Expr::Atom const *>(exprs.front().get());
  EXPECT_EQ(atom->value<Expr::Symbol>().name, "foo");
}

TEST(Parser, List) {
  Lexer lexer("(+ 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_TRUE(nullptr != exprs.front());
  Expr::List const *list =
      dynamic_cast<Expr::List const *>(exprs.front().get());
  EXPECT_TRUE(nullptr != list);
  EXPECT_EQ(list->exprs().size(), 3);
  auto i = list->exprs().begin();
  auto atom = dynamic_cast<Expr::Atom const *>(i->get());
  EXPECT_EQ(atom->value<Expr::Symbol>().name, "+");
  i++;
  atom = dynamic_cast<Expr::Atom const *>(i->get());
  EXPECT_EQ(atom->value<long>(), 1);
  i++;
  atom = dynamic_cast<Expr::Atom const *>(i->get());
  EXPECT_EQ(atom->value<long>(), 2);
}

TEST(Parser, DefConstructor) {
  std::unique_ptr<Expr> def =
      std::make_unique<Expr::Def>("foo", std::make_unique<Expr::Atom>(42));
  auto defptr = dynamic_cast<Expr::Def const *>(def.get());
  EXPECT_EQ("foo", defptr->name());
}

TEST(Parser, Def) {
  Lexer lexer("(def foo 42)");
  auto tokens = lexer.scan();
  EXPECT_EQ(6, tokens.size());
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
  auto def = dynamic_cast<Expr::Def const *>(exprs.front().get());
  EXPECT_TRUE(nullptr != def);
  EXPECT_EQ("foo", def->name());
}

TEST(Parser, Let) {
  Lexer lexer("(let [a 2] a)");
  auto tokens = lexer.scan();
  EXPECT_EQ(9, tokens.size());
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(1, exprs.size());
}
// Local Variables:
// compile-command : "bazel test //oxid:parser_test"
// End:
