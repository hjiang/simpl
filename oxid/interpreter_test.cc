#include "interpreter.h"

#include <gtest/gtest.h>

#include <memory>

#include "lexer.h"

using namespace oxid;

TEST(Parser, Integer) {
  Lexer lexer("42");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<long>(interpreter.evaluate(expr)), 42);
}

TEST(Parser, Bool) {
  Lexer lexer("true");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.evaluate(expr)));
}

TEST(Parser, Plus) {
  Lexer lexer("(+ 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<long>(interpreter.evaluate(expr)), 3);
}

TEST(Parser, NestedSum) {
  Lexer lexer("(+ 1 2 (+ 3 4))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<long>(interpreter.evaluate(expr)), 10);
}

TEST(Parser, UnarySum) {
  Lexer lexer("(+ 42)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<long>(interpreter.evaluate(expr)), 42);
}

TEST(Parser, Substract) {
  Lexer lexer("(- 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<long>(interpreter.evaluate(expr)), -1);
}

TEST(Parser, UnarySubstract) {
  Lexer lexer("(- 12)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<long>(interpreter.evaluate(expr)), -12);
}

TEST(Parser, InterpretProgram) {
  Lexer lexer("(- 1 2)\n(+ 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<long>(interpreter.evaluate(expr)), 7);
}
// Local Variables:
// compile-command : "bazel test //oxid:all"
// End:
