// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.h"

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>

#include "simpl/ast.h"
#include "simpl/interpreter_util.h"
#include "simpl/lexer.h"
#include "simpl/parser.h"

namespace simpl {

TEST(Interpreter, Integer) {
  Lexer lexer("42");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 42);
}

TEST(Interpreter, Bool) {
  Lexer lexer("true");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.Evaluate(expr)));
}

TEST(Interpreter, Plus) {
  Lexer lexer("(+ 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(exprs.size(), 1);
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(exprs)), 3);
}

TEST(Interpreter, Quoted) {
  Lexer lexer("'(+ 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(exprs.size(), 1);
  Interpreter interpreter;
  EXPECT_TRUE(holds<List>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, QuotedSymbol) {
  Lexer lexer("'(let [a 'b] (cons a '(1 2)))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  EXPECT_EQ(exprs.size(), 1);
  Interpreter interpreter;
  EXPECT_TRUE(holds<List>(interpreter.Evaluate(exprs)));
  EXPECT_EQ(std::get<List>(interpreter.Evaluate(exprs)).size(), 3);
}

TEST(Interpreter, NestedSum) {
  Lexer lexer("(+ 1 2 (+ 3 4))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 10);
}

TEST(Interpreter, UnarySum) {
  Lexer lexer("(+ 42)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 42);
}

TEST(Interpreter, Substract) {
  Lexer lexer("(- 1 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), -1);
}

TEST(Interpreter, MoreSubstract) {
  Lexer lexer("(- 1 2 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), -8);
}

TEST(Interpreter, UnarySubstract) {
  Lexer lexer("(- 12)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), -12);
}

TEST(Interpreter, Multiply) {
  Lexer lexer("(* 3 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 6);
}

TEST(Interpreter, MoreMultiply) {
  Lexer lexer("(* 2 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 24);
}

TEST(Interpreter, divide) {
  Lexer lexer("(/ 6 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 3);
}

TEST(Interpreter, mod) {
  Lexer lexer("(% 11 3)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 2);
}

TEST(Interpreter, StringCompareNotEqual) {
  Lexer lexer("(= \"foobar\" \"foo-bar\")");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_FALSE(std::get<bool>(interpreter.Evaluate(expr)));
}

TEST(Interpreter, InterpretProgram) {
  Lexer lexer("(- 1 2)\n(+ 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 7);
}

TEST(Interpreter, DefineVar) {
  Lexer lexer("(def a 2)\n(+ a 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 6);
}

TEST(Interpreter, Let) {
  Lexer lexer("(let [a 3] (+ a 4))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 7);
}

TEST(Interpreter, NestedLet) {
  Lexer lexer("(let [a 2] (let [a 3] (+ a 4)) a)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 2);
}

TEST(Interpreter, If) {
  Lexer lexer("(if true 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(exprs)), 3);
  Lexer lexer2("(if false 3 4)");
  Parser parser2(lexer2.scan());
  exprs = parser2.Parse();
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(exprs)), 4);
}

TEST(Interpreter, Not) {
  Lexer lexer("(not nil)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, EqualityTrue) {
  Lexer lexer("(= 2 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, EqualityFalse) {
  Lexer lexer("(= 2 3)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_FALSE(std::get<bool>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, GreaterThan) {
  Lexer lexer("(> 3.2 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, GreaterThan_False) {
  Lexer lexer("(> 3 5)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_FALSE(std::get<bool>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, LessThan_False) {
  Lexer lexer("(< 3.2 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_FALSE(std::get<bool>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, LessThan) {
  Lexer lexer("(< 3 5)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto exprs = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.Evaluate(exprs)));
}

TEST(Interpreter, LetAndFn) {
  Lexer lexer("(let [plus (fn [a b] (+ a b))] (plus 3 4))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 7);
}

TEST(Interpreter, Closure) {
  Lexer lexer("(let [a 5] (def plus (fn [b] (+ a b)))) (plus 2)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 7);
}

TEST(Interpreter, Defn) {
  Lexer lexer("(defn plus [a b] (+ a b)) (plus 3 4)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(std::get<int_type>(interpreter.Evaluate(expr)), 7);
}

TEST(Interpreter, Or) {
  Lexer lexer("(or true false)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(std::get<bool>(interpreter.Evaluate(expr)));
}

TEST(Interpreter, And) {
  Lexer lexer("(and true false)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_FALSE(std::get<bool>(interpreter.Evaluate(expr)));
}

TEST(Interpreter, EmptyListIsNil) {
  Lexer lexer("()");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_TRUE(nullptr == std::get<std::nullptr_t>(interpreter.Evaluate(expr)));
}

TEST(Interpreter, Vector) {
  Lexer lexer("[1 2 3]");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  auto vec = std::get<Vector>(interpreter.Evaluate(expr));
  EXPECT_EQ(1, std::get<int_type>(vec.front()));
}

TEST(Interpreter, Do) {
  Lexer lexer("(do 12 42)");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(42, std::get<int_type>(interpreter.Evaluate(expr)));
}

TEST(Interpreter, AssertTrue) {
  Lexer lexer("(assert (= 2 2) \"True\")");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_NO_THROW(interpreter.Evaluate(expr));
}

TEST(Interpreter, AssertFalse) {
  Lexer lexer("(assert (> 2 3.0) \"True\")");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_THROW(interpreter.Evaluate(expr), std::runtime_error);
}

TEST(Interpreter, GetMapKey) {
  Lexer lexer("(let [m {:a 1 :b 2}] (get m :a))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(1, std::get<int_type>(interpreter.Evaluate(expr)));
}

TEST(Interpreter, KeyMap) {
  Lexer lexer("(let [m {:a 1 :b 2}] (:a m))");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto expr = parser.Parse();
  Interpreter interpreter;
  EXPECT_EQ(1, std::get<int_type>(interpreter.Evaluate(expr)));
}

}  // namespace simpl
