#include "parser.h"

#include <gtest/gtest.h>

#include <memory>

#include "lexer.h"

using namespace oxid;

TEST(Parser, Integer) {
  Lexer lexer("1");
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto atom = std::dynamic_pointer_cast<Expr::Atom>(parser.Parse());
  EXPECT_EQ(atom->value<long>(), 1);
}

// Local Variables:
// compile-command : "bazel test //oxid:parser_test"
// End:
