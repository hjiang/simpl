// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.h"

#include <gtest/gtest.h>

#include "simpl/ast.h"
#include "simpl/config.h"
#include "simpl/interpreter_util.h"

namespace simpl {

TEST(Simpl, SimpleExpression) {
  auto e = run("(+ 2 3)");
  EXPECT_TRUE(holds<int_type>(e));
  EXPECT_EQ(std::get<int_type>(e), 5);
}

TEST(Simpl, Cons) {
  auto e = run("(cons 2 nil)");
  EXPECT_TRUE(holds<list_ptr_t>(e));
}
TEST(Simpl, ConsEmpty) {
  auto e = run("(cons 2 '())");
  EXPECT_TRUE(holds<list_ptr_t>(e));
}

TEST(Simpl, ConsList) {
  auto e = run("(cons 1 '(2 3))");
  EXPECT_TRUE(holds<list_ptr_t>(e));
}

}  // namespace simpl
