// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.hh"

#include <gtest/gtest.h>

#include <stdexcept>

#include "simpl/ast.hh"
#include "simpl/config.hh"
#include "simpl/interpreter_util.hh"

namespace simpl {

TEST(SimplTest, SimpleExpression) {
  auto e = run("(+ 2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 5);
}

TEST(SimplTest, ExplicitPositiveNumber) {
  auto e = run("(+ +2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 5);
}

TEST(SimplTest, NegativeNumber) {
  auto e = run("(+ -2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 1);
}

TEST(SimplTest, IncompleteExpression) {
  EXPECT_THROW(run("(+ 2 3"), std::runtime_error);
}

TEST(SimplTest, ExtraToken) {
  EXPECT_THROW(run("(+ 2 3))"), std::runtime_error);
}

TEST(SimplTest, InvalidToken) {
  EXPECT_THROW(run("(+ 2 3#)"), std::runtime_error);
}

TEST(SimplTest, UnterminatedString) {
  EXPECT_THROW(run("(+ \"foo\" \"ba)"), std::runtime_error);
}

TEST(SimplTest, InvalidArgument) {
  EXPECT_THROW(run("(+ \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(- \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(* \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(/ \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(% \"foo\" 32)"), std::runtime_error);
}

TEST(SimplTest, AndWithoutArguments) { EXPECT_EQ(run("(and)"), Expr{true}); }

TEST(SimplTest, OrWithoutArguments) { EXPECT_EQ(run("(or)"), Expr{false}); }

TEST(SimplTest, OrReturnsLastFalseArgument) {
  auto e = run("(or false nil ())");
  EXPECT_TRUE(holds<std::nullptr_t>(e));
}

TEST(SimplTest, AndReturnsLastTrueArgument) {
  auto e = run("(and 1 2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 3);
}

TEST(SimplTest, Cons) {
  auto e = run("(cons 2 nil)");
  EXPECT_TRUE(holds<List>(e));
}

TEST(SimplTest, ConsWrongArgCount) {
  EXPECT_THROW(run("(cons 2 3 4)"), std::runtime_error);
}

TEST(SimplTest, ConsWrongArg) {
  EXPECT_THROW(run("(cons 2 3)"), std::runtime_error);
}

TEST(SimplTest, ConsEmpty) {
  auto e = run("(cons 2 '())");
  EXPECT_TRUE(holds<List>(e));
}

TEST(SimplTest, ConsList) {
  auto e = run("(cons 1 '(2 3))");
  EXPECT_TRUE(holds<List>(e));
}

TEST(SimplTest, ListHead) {
  auto e = run("(head '(1 2 3))");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 1);
}

TEST(SimplTest, ListTail) {
  auto e = run("(tail '(1 2 3))");
  EXPECT_TRUE(holds<List>(e));
  EXPECT_EQ(std::get<List>(e).size(), 2);
}

TEST(SimplTest, VectorHead) {
  auto e = run("(head [1 2 3])");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 1);
}

TEST(SimplTest, VectorTail) {
  auto e = run("(tail [1 2 3])");
  EXPECT_TRUE(holds<Vector>(e));
  EXPECT_EQ(std::get<int_type>(std::get<Vector>(e).front()), 2);
}

TEST(SimplTest, VectorGet) {
  auto e = run("(get [1 2 3] 2)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 3);
}

TEST(SimplTest, ListCannotGet) {
  EXPECT_THROW(run("(get '(1 2 3) 2)"), std::runtime_error);
}

TEST(SimplTest, CompareIntAndString) {
  EXPECT_THROW(run("(< 3 \"3\")"), std::runtime_error);
}

TEST(SimplTest, CannotCompare3things) {
  EXPECT_THROW(run("(< 3 4 5)"), std::runtime_error);
}

TEST(SimplTest, InvalidIf) {
  EXPECT_THROW(run("(if true 1 2 3)"), std::runtime_error);
}

TEST(SimplTest, InvalidLet) {
  EXPECT_THROW(run("(let [a 1 b] a)"), std::runtime_error);
}

TEST(SimplTest, InvalidFn) { EXPECT_THROW(run("(fn)"), std::runtime_error); }

TEST(SimplTest, InvalidSequenceOp) {
  EXPECT_THROW(run("(head 123)"), std::runtime_error);
  EXPECT_THROW(run("(tail 123)"), std::runtime_error);
}

TEST(SimplTest, UndefinedVar) {
  EXPECT_THROW(run("(cons a '(2 3))"), std::runtime_error);
}

TEST(SimplTest, UndefinedFn) {
  EXPECT_THROW(run("(foo 1 2 3)"), std::runtime_error);
}

TEST(SimplTest, InvalidDef) {
  EXPECT_THROW(run("(def a 1 2)"), std::runtime_error);
  EXPECT_THROW(run("(defn foo)"), std::runtime_error);
}

}  // namespace simpl
