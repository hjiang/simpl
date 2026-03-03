// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.hh"

#include <gtest/gtest.h>

#include <stdexcept>

#include "simpl/ast.hh"
#include "simpl/interpreter_util.hh"
#include "simpl/lexer.hh"
#include "simpl/parser.hh"

namespace simpl {

class InterpreterTest : public ::testing::Test {
 protected:
  Expr Eval(const std::string& source) {
    Lexer lexer(source);
    Parser parser(lexer.scan());
    return interpreter_.Evaluate(parser.Parse());
  }
  Interpreter interpreter_;
};

TEST_F(InterpreterTest, Integer) {
  EXPECT_EQ(std::get<int_type>(Eval("42")), 42);
}

TEST_F(InterpreterTest, Bool) {
  EXPECT_TRUE(std::get<bool>(Eval("true")));
}

TEST_F(InterpreterTest, Plus) {
  EXPECT_EQ(std::get<int_type>(Eval("(+ 1 2)")), 3);
}

TEST_F(InterpreterTest, Quoted) {
  EXPECT_TRUE(holds<List>(Eval("'(+ 1 2)")));
}

TEST_F(InterpreterTest, QuotedSymbol) {
  auto res = Eval("'(let [a 'b] (cons a '(1 2)))");
  EXPECT_TRUE(holds<List>(res));
  EXPECT_EQ(std::get<List>(res).size(), 3);
}

TEST_F(InterpreterTest, NestedSum) {
  EXPECT_EQ(std::get<int_type>(Eval("(+ 1 2 (+ 3 4))")), 10);
}

TEST_F(InterpreterTest, UnarySum) {
  EXPECT_EQ(std::get<int_type>(Eval("(+ 42)")), 42);
}

TEST_F(InterpreterTest, Subtract) {
  EXPECT_EQ(std::get<int_type>(Eval("(- 1 2)")), -1);
}

TEST_F(InterpreterTest, MoreSubtract) {
  EXPECT_EQ(std::get<int_type>(Eval("(- 1 2 3 4)")), -8);
}

TEST_F(InterpreterTest, UnarySubtract) {
  EXPECT_EQ(std::get<int_type>(Eval("(- 12)")), -12);
}

TEST_F(InterpreterTest, Multiply) {
  EXPECT_EQ(std::get<int_type>(Eval("(* 3 2)")), 6);
}

TEST_F(InterpreterTest, MoreMultiply) {
  EXPECT_EQ(std::get<int_type>(Eval("(* 2 3 4)")), 24);
}

TEST_F(InterpreterTest, Divide) {
  EXPECT_EQ(std::get<int_type>(Eval("(/ 6 2)")), 3);
}

TEST_F(InterpreterTest, Mod) {
  EXPECT_EQ(std::get<int_type>(Eval("(% 11 3)")), 2);
}

TEST_F(InterpreterTest, StringCompareNotEqual) {
  EXPECT_FALSE(std::get<bool>(Eval("(= \"foobar\" \"foo-bar\")")));
}

TEST_F(InterpreterTest, InterpretProgram) {
  EXPECT_EQ(std::get<int_type>(Eval("(- 1 2)\n(+ 3 4)")), 7);
}

TEST_F(InterpreterTest, DefineVar) {
  EXPECT_EQ(std::get<int_type>(Eval("(def a 2)\n(+ a 4)")), 6);
}

TEST_F(InterpreterTest, Let) {
  EXPECT_EQ(std::get<int_type>(Eval("(let [a 3] (+ a 4))")), 7);
}

TEST_F(InterpreterTest, NestedLet) {
  EXPECT_EQ(std::get<int_type>(Eval("(let [a 2] (let [a 3] (+ a 4)) a)")), 2);
}

TEST_F(InterpreterTest, If) {
  EXPECT_EQ(std::get<int_type>(Eval("(if true 3 4)")), 3);
  EXPECT_EQ(std::get<int_type>(Eval("(if false 3 4)")), 4);
}

TEST_F(InterpreterTest, Not) {
  EXPECT_TRUE(std::get<bool>(Eval("(not nil)")));
}

TEST_F(InterpreterTest, EqualityTrue) {
  EXPECT_TRUE(std::get<bool>(Eval("(= 2 2)")));
}

TEST_F(InterpreterTest, EqualityFalse) {
  EXPECT_FALSE(std::get<bool>(Eval("(= 2 3)")));
}

TEST_F(InterpreterTest, GreaterThan) {
  EXPECT_TRUE(std::get<bool>(Eval("(> 3.2 2)")));
}

TEST_F(InterpreterTest, GreaterThanFalse) {
  EXPECT_FALSE(std::get<bool>(Eval("(> 3 5)")));
}

TEST_F(InterpreterTest, LessThanFalse) {
  EXPECT_FALSE(std::get<bool>(Eval("(< 3.2 2)")));
}

TEST_F(InterpreterTest, LessThan) {
  EXPECT_TRUE(std::get<bool>(Eval("(< 3 5)")));
}

TEST_F(InterpreterTest, LetAndFn) {
  EXPECT_EQ(std::get<int_type>(Eval("(let [plus (fn [a b] (+ a b))] (plus 3 4))")), 7);
}

TEST_F(InterpreterTest, Closure) {
  EXPECT_EQ(std::get<int_type>(Eval("(let [a 5] (def plus (fn [b] (+ a b)))) (plus 2)")), 7);
}

TEST_F(InterpreterTest, Defn) {
  EXPECT_EQ(std::get<int_type>(Eval("(defn plus [a b] (+ a b)) (plus 3 4)")), 7);
}

TEST_F(InterpreterTest, DefnVariadic) {
  EXPECT_TRUE(holds<std::nullptr_t>(Eval("(defn foo [a & b] (head b))")));
}

TEST_F(InterpreterTest, EvalVariadic) {
  EXPECT_EQ(std::get<int_type>(Eval("(defn foo [a & b] (head b)) (foo 1 2 3)")), 2);
}

TEST_F(InterpreterTest, EmptyRestArgs) {
  EXPECT_TRUE(std::get<bool>(Eval("(defn foo [a & b] (empty? b)) (foo 1)")));
}

TEST_F(InterpreterTest, Or) {
  EXPECT_TRUE(std::get<bool>(Eval("(or true false)")));
}

TEST_F(InterpreterTest, And) {
  EXPECT_FALSE(std::get<bool>(Eval("(and true false)")));
}

TEST_F(InterpreterTest, EmptyListIsNil) {
  EXPECT_TRUE(nullptr == std::get<std::nullptr_t>(Eval("()")));
}

TEST_F(InterpreterTest, Vector) {
  auto vec = std::get<Vector>(Eval("[1 2 3]"));
  EXPECT_EQ(1, std::get<int_type>(vec.front()));
}

TEST_F(InterpreterTest, Do) {
  EXPECT_EQ(42, std::get<int_type>(Eval("(do 12 42)")));
}

TEST_F(InterpreterTest, AssertTrue) {
  EXPECT_NO_THROW(Eval("(assert (= 2 2) \"True\")"));
}

TEST_F(InterpreterTest, AssertFalse) {
  EXPECT_THROW(Eval("(assert (> 2 3.0) \"True\")"), std::runtime_error);
}

TEST_F(InterpreterTest, GetMapKey) {
  EXPECT_EQ(1, std::get<int_type>(Eval("(let [m {:a 1 :b 2}] (get m :a))")));
}

TEST_F(InterpreterTest, KeyMap) {
  EXPECT_EQ(1, std::get<int_type>(Eval("(let [m {:a 1 :b 2}] (:a m))")));
}

TEST_F(InterpreterTest, LazyFn) {
  EXPECT_EQ(2, std::get<int_type>(
                   Eval("(let [f (lazy-fn [a] (eval (head a)))] "
                        "(f ((/ 2 1) (/ 2 0))))")));
}

TEST_F(InterpreterTest, Empty) {
  EXPECT_TRUE(std::get<bool>(Eval("(empty? [])")));
}

TEST_F(InterpreterTest, NotEmpty) {
  EXPECT_FALSE(std::get<bool>(Eval("(empty? [1])")));
}

TEST_F(InterpreterTest, CallTwice) {
  EXPECT_EQ(std::get<int_type>(Eval("(defn foo [a] (+ 1 a)) (foo 1) (foo 1)")), 2);
}

// Stage 3: Coverage gap tests

TEST_F(InterpreterTest, GreaterThanOrEqual) {
  EXPECT_TRUE(std::get<bool>(Eval("(>= 3 3)")));
  EXPECT_TRUE(std::get<bool>(Eval("(>= 4 3)")));
  EXPECT_FALSE(std::get<bool>(Eval("(>= 2 3)")));
}

TEST_F(InterpreterTest, LessThanOrEqual) {
  EXPECT_TRUE(std::get<bool>(Eval("(<= 3 3)")));
  EXPECT_TRUE(std::get<bool>(Eval("(<= 2 3)")));
  EXPECT_FALSE(std::get<bool>(Eval("(<= 4 3)")));
}

TEST_F(InterpreterTest, DivisionByZero) {
  EXPECT_THROW(Eval("(/ 1 0)"), std::runtime_error);
}

TEST_F(InterpreterTest, Recursion) {
  EXPECT_EQ(std::get<int_type>(Eval(
                "(defn fact [n] (if (= n 0) 1 (* n (fact (- n 1))))) "
                "(fact 5)")),
            120);
}

TEST_F(InterpreterTest, FloatArithmetic) {
  EXPECT_DOUBLE_EQ(std::get<float_type>(Eval("(+ 1.5 2.5)")), 4.0);
  EXPECT_DOUBLE_EQ(std::get<float_type>(Eval("(* 2.0 3.5)")), 7.0);
}

TEST_F(InterpreterTest, LetMultipleBindings) {
  EXPECT_EQ(std::get<int_type>(Eval("(let [a 1 b 2] (+ a b))")), 3);
}

TEST_F(InterpreterTest, StringEqual) {
  EXPECT_TRUE(std::get<bool>(Eval("(= \"foo\" \"foo\")")));
}

}  // namespace simpl
