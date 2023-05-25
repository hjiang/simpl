// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.h"

#include <gtest/gtest.h>

#include <memory>

#include "simpl/lexer.h"

namespace simpl {

TEST(Parser, Integer) {
  Lexer lexer("42");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 42);
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
  auto exprs = parser.Parse();
  EXPECT_EQ(exprs.size(), 1);
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(*exprs.front())), 3);
}

TEST(Parser, NestedSum) {
  Lexer lexer("(+ 1 2 (+ 3 4))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 10);
}

TEST(Parser, UnarySum) {
  Lexer lexer("(+ 42)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 42);
}

TEST(Parser, Substract) {
  Lexer lexer("(- 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), -1);
}

TEST(Parser, MoreSubstract) {
  Lexer lexer("(- 1 2 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), -8);
}

TEST(Parser, UnarySubstract) {
  Lexer lexer("(- 12)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), -12);
}

TEST(Parser, Multiply) {
  Lexer lexer("(* 3 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 6);
}

TEST(Parser, MoreMultiply) {
  Lexer lexer("(* 2 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 24);
}

TEST(Parser, divide) {
  Lexer lexer("(/ 6 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 3);
}

TEST(Parser, mod) {
  Lexer lexer("(% 11 3)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 2);
}

TEST(Parser, InterpretProgram) {
  Lexer lexer("(- 1 2)\n(+ 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 7);
}

TEST(Parser, DefineVar) {
  Lexer lexer("(def a 2)\n(+ a 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 6);
}

TEST(Parser, Let) {
  Lexer lexer("(let [a 3] (+ a 4))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 7);
}

TEST(Parser, NestedLet) {
  Lexer lexer("(let [a 2] (let [a 3] (+ a 4)) a)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(expr)), 2);
}

TEST(Parser, If) {
  Lexer lexer("(if true 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(exprs)), 3);
  Lexer lexer2("(if false 3 4)");
  Parser parser2(lexer2.scan());
  exprs = parser2.Parse();
  EXPECT_EQ(std::get<int_type>(interpreter.evaluate(exprs)), 4);
}

TEST(Parser, Not) {
  Lexer lexer("(not nil)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.evaluate(exprs)));
}

}  // namespace simpl
// Local Variables:
// compile-command : "bazel test //simpl:all"
// End:
