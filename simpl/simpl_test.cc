// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.hh"

#include <gtest/gtest.h>

#include <stdexcept>

#include "simpl/ast.hh"
#include "simpl/config.hh"
#include "simpl/error.hh"
#include "simpl/interpreter_util.hh"

namespace simpl {

class SimplTest : public ::testing::Test {
 protected:
  void SetUp() override {
    ClearError();
    ClearRuntimeError();
  }
};

TEST_F(SimplTest, SimpleExpression) {
  auto e = run("(+ 2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 5);
}

TEST_F(SimplTest, ExplicitPositiveNumber) {
  auto e = run("(+ +2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 5);
}

TEST_F(SimplTest, NegativeNumber) {
  auto e = run("(+ -2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 1);
}

TEST_F(SimplTest, IncompleteExpression) {
  EXPECT_THROW(run("(+ 2 3"), std::runtime_error);
}

TEST_F(SimplTest, ExtraToken) {
  EXPECT_THROW(run("(+ 2 3))"), std::runtime_error);
}

TEST_F(SimplTest, InvalidToken) {
  EXPECT_THROW(run("(+ 2 3#)"), std::runtime_error);
}

TEST_F(SimplTest, UnterminatedString) {
  EXPECT_THROW(run("(+ \"foo\" \"ba)"), std::runtime_error);
}

TEST_F(SimplTest, InvalidArgument) {
  EXPECT_THROW(run("(+ \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(- \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(* \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(/ \"foo\" 32)"), std::runtime_error);
  EXPECT_THROW(run("(% \"foo\" 32)"), std::runtime_error);
}

TEST_F(SimplTest, AndWithoutArguments) { EXPECT_EQ(run("(and)"), Expr{true}); }

TEST_F(SimplTest, OrWithoutArguments) { EXPECT_EQ(run("(or)"), Expr{false}); }

TEST_F(SimplTest, OrReturnsLastFalseArgument) {
  auto e = run("(or false nil ())");
  EXPECT_TRUE(holds<std::nullptr_t>(e));
}

TEST_F(SimplTest, AndReturnsLastTrueArgument) {
  auto e = run("(and 1 2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 3);
}

TEST_F(SimplTest, Cons) {
  auto e = run("(cons 2 nil)");
  EXPECT_TRUE(holds<List>(e));
}

TEST_F(SimplTest, ConsWrongArgCount) {
  EXPECT_THROW(run("(cons 2 3 4)"), std::runtime_error);
}

TEST_F(SimplTest, ConsWrongArg) {
  EXPECT_THROW(run("(cons 2 3)"), std::runtime_error);
}

TEST_F(SimplTest, ConsEmpty) {
  auto e = run("(cons 2 '())");
  EXPECT_TRUE(holds<List>(e));
}

TEST_F(SimplTest, ConsList) {
  auto e = run("(cons 1 '(2 3))");
  EXPECT_TRUE(holds<List>(e));
}

TEST_F(SimplTest, ListHead) {
  auto e = run("(head '(1 2 3))");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 1);
}

TEST_F(SimplTest, ListTail) {
  auto e = run("(tail '(1 2 3))");
  EXPECT_TRUE(holds<List>(e));
  EXPECT_EQ(std::get<List>(e).size(), 2);
}

TEST_F(SimplTest, VectorHead) {
  auto e = run("(head [1 2 3])");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 1);
}

TEST_F(SimplTest, VectorTail) {
  auto e = run("(tail [1 2 3])");
  EXPECT_TRUE(holds<Vector>(e));
  EXPECT_EQ(std::get<int_type>(std::get<Vector>(e).front()), 2);
}

TEST_F(SimplTest, VectorGet) {
  auto e = run("(get [1 2 3] 2)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 3);
}

TEST_F(SimplTest, ListCannotGet) {
  EXPECT_THROW(run("(get '(1 2 3) 2)"), std::runtime_error);
}

TEST_F(SimplTest, CompareIntAndString) {
  EXPECT_THROW(run("(< 3 \"3\")"), std::runtime_error);
}

TEST_F(SimplTest, CannotCompare3things) {
  EXPECT_THROW(run("(< 3 4 5)"), std::runtime_error);
}

TEST_F(SimplTest, InvalidIf) {
  EXPECT_THROW(run("(if true 1 2 3)"), std::runtime_error);
}

TEST_F(SimplTest, InvalidLet) {
  EXPECT_THROW(run("(let [a 1 b] a)"), std::runtime_error);
}

TEST_F(SimplTest, InvalidFn) { EXPECT_THROW(run("(fn)"), std::runtime_error); }

TEST_F(SimplTest, InvalidSequenceOp) {
  EXPECT_THROW(run("(head 123)"), std::runtime_error);
  EXPECT_THROW(run("(tail 123)"), std::runtime_error);
}

TEST_F(SimplTest, UndefinedVar) {
  EXPECT_THROW(run("(cons a '(2 3))"), std::runtime_error);
}

TEST_F(SimplTest, UndefinedFn) {
  EXPECT_THROW(run("(foo 1 2 3)"), std::runtime_error);
}

TEST_F(SimplTest, InvalidDef) {
  EXPECT_THROW(run("(def a 1 2)"), std::runtime_error);
  EXPECT_THROW(run("(defn foo)"), std::runtime_error);
}

}  // namespace simpl
