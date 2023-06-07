// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.h"

#include <gtest/gtest.h>

#include <stdexcept>

#include "simpl/ast.h"
#include "simpl/config.h"
#include "simpl/error.h"
#include "simpl/interpreter_util.h"

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

TEST_F(SimplTest, IncompleteExpression) {
  EXPECT_THROW(run("(+ 2 3"), std::runtime_error);
}

TEST_F(SimplTest, ExtraToken) {
  EXPECT_THROW(run("(+ 2 3))"), std::runtime_error);
}

TEST_F(SimplTest, InvalidToken) {
  EXPECT_THROW(run("(+ 2 3#)"), std::runtime_error);
}

TEST_F(SimplTest, AndExpectsArguments) {
  EXPECT_THROW(run("(and)"), std::runtime_error);
}

TEST_F(SimplTest, OrExpectsArguments) {
  EXPECT_THROW(run("(and)"), std::runtime_error);
}

TEST_F(SimplTest, OrReturnsLastFalseArgument) {
  auto e = run("(or false nil ())");
  EXPECT_TRUE(holds<nullptr_t>(e));
}

TEST_F(SimplTest, AndReturnsLastTrueArgument) {
  auto e = run("(and 1 2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 3);
}

TEST_F(SimplTest, Cons) {
  auto e = run("(cons 2 nil)");
  EXPECT_TRUE(holds<list_ptr_t>(e));
}

TEST_F(SimplTest, ConsEmpty) {
  auto e = run("(cons 2 '())");
  EXPECT_TRUE(holds<list_ptr_t>(e));
}

TEST_F(SimplTest, ConsList) {
  auto e = run("(cons 1 '(2 3))");
  EXPECT_TRUE(holds<list_ptr_t>(e));
}

}  // namespace simpl
