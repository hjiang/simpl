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

// Local Variables:
// compile-command : "bazel test //oxid:all"
// End:
