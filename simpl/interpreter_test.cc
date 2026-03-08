// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/interpreter.hh"

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

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

TEST_F(InterpreterTest, Bool) { EXPECT_TRUE(std::get<bool>(Eval("true"))); }

TEST_F(InterpreterTest, Plus) {
  EXPECT_EQ(std::get<int_type>(Eval("(+ 1 2)")), 3);
}

TEST_F(InterpreterTest, Quoted) { EXPECT_TRUE(holds<List>(Eval("'(+ 1 2)"))); }

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

TEST_F(InterpreterTest, Not) { EXPECT_TRUE(std::get<bool>(Eval("(not nil)"))); }

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
  EXPECT_EQ(
      std::get<int_type>(Eval("(let [plus (fn [a b] (+ a b))] (plus 3 4))")),
      7);
}

TEST_F(InterpreterTest, Closure) {
  EXPECT_EQ(std::get<int_type>(
                Eval("(let [a 5] (def plus (fn [b] (+ a b)))) (plus 2)")),
            7);
}

TEST_F(InterpreterTest, Defn) {
  EXPECT_EQ(std::get<int_type>(Eval("(defn plus [a b] (+ a b)) (plus 3 4)")),
            7);
}

TEST_F(InterpreterTest, DefnVariadic) {
  EXPECT_TRUE(holds<std::nullptr_t>(Eval("(defn foo [a & b] (head b))")));
}

TEST_F(InterpreterTest, EvalVariadic) {
  EXPECT_EQ(std::get<int_type>(Eval("(defn foo [a & b] (head b)) (foo 1 2 3)")),
            2);
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
  EXPECT_EQ(2, std::get<int_type>(Eval("(let [f (lazy-fn [a] (eval (head a)))] "
                                       "(f ((/ 2 1) (/ 2 0))))")));
}

TEST_F(InterpreterTest, Empty) {
  EXPECT_TRUE(std::get<bool>(Eval("(empty? [])")));
}

TEST_F(InterpreterTest, NotEmpty) {
  EXPECT_FALSE(std::get<bool>(Eval("(empty? [1])")));
}

TEST_F(InterpreterTest, CallTwice) {
  EXPECT_EQ(std::get<int_type>(Eval("(defn foo [a] (+ 1 a)) (foo 1) (foo 1)")),
            2);
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
  EXPECT_EQ(std::get<int_type>(
                Eval("(defn fact [n] (if (= n 0) 1 (* n (fact (- n 1))))) "
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

TEST_F(InterpreterTest, TailCallOptimizationSelfRecursion) {
  // Would stack-overflow without TCO at n=1,000,000
  EXPECT_EQ(
      std::get<int_type>(Eval("(defn countdown [n] "
                              "  (if (= n 0) 0 (countdown (- n 1)))) "
                              "(countdown 1000000)")),
      int_type{0});
}

// Macro system tests

TEST_F(InterpreterTest, SyntaxQuoteReturnsList) {
  EXPECT_TRUE(holds<List>(Eval("`(a b c)")));
  auto list = std::get<List>(Eval("`(a b c)"));
  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(std::get<Symbol>(list.front()).name, "a");
}

TEST_F(InterpreterTest, SyntaxQuoteWithUnquote) {
  EXPECT_EQ(std::get<int_type>(
                Eval("(def x 42) "
                     "(let [result `(a ~x c)] (head (tail result)))")),
            42);
}

TEST_F(InterpreterTest, SyntaxQuoteWithSplice) {
  auto result = Eval("(def args '(1 2 3)) `(+ ~@args)");
  auto list = std::get<List>(result);
  EXPECT_EQ(list.size(), 4);
  EXPECT_EQ(std::get<Symbol>(list.front()).name, "+");
}

TEST_F(InterpreterTest, SpliceOutsideSyntaxQuote) {
  EXPECT_THROW(Eval("~@x"), std::runtime_error);
}

TEST_F(InterpreterTest, DefmacroAndUsage) {
  EXPECT_EQ(
      std::get<int_type>(
          Eval("(defmacro when [cond & body] `(if ~cond (do ~@body) nil))"
               "(when true (+ 1 2))")),
      3);
}

TEST_F(InterpreterTest, DefmacroFalseCondition) {
  EXPECT_TRUE(holds<std::nullptr_t>(
      Eval("(defmacro when [cond & body] `(if ~cond (do ~@body) nil))"
           "(when false (+ 1 2))")));
}

TEST_F(InterpreterTest, DefmacroMultipleBody) {
  EXPECT_EQ(
      std::get<int_type>(
          Eval("(defmacro when [cond & body] `(if ~cond (do ~@body) nil))"
               "(when true 1 2 3)")),
      3);
}

TEST_F(InterpreterTest, MacroexpandReturnsExpandedForm) {
  auto result =
      Eval("(defmacro when [cond & body] `(if ~cond (do ~@body) nil))"
           "(macroexpand '(when true 1 2))");
  EXPECT_TRUE(holds<List>(result));
  auto list = std::get<List>(result);
  EXPECT_EQ(list.size(), 4);
  EXPECT_EQ(std::get<Symbol>(list.front()).name, "if");
}

TEST_F(InterpreterTest, MacroAccessesCallerScope) {
  EXPECT_EQ(
      std::get<int_type>(
          Eval("(defmacro when [cond & body] `(if ~cond (do ~@body) nil))"
               "(def x 10) (when true x)")),
      10);
}

// Cross-numeric equality (ExprEqual int-vs-float branches)
TEST_F(InterpreterTest, CrossNumericEquality) {
  EXPECT_TRUE(std::get<bool>(Eval("(= 1 1.0)")));
  EXPECT_TRUE(std::get<bool>(Eval("(= 1.0 1)")));
  EXPECT_FALSE(std::get<bool>(Eval("(= 1 2.0)")));
}

// ~x at the top level of a syntax-quote (ProcessSyntaxQuote Quoted handler,
// kUnquote branch)
TEST_F(InterpreterTest, SyntaxQuoteTopLevelUnquote) {
  EXPECT_EQ(std::get<int_type>(Eval("(def x 42) `~x")), 42);
}

// ~@x at the top level of a syntax-quote (ProcessSyntaxQuote Quoted handler,
// kSplice error branch)
TEST_F(InterpreterTest, SyntaxQuoteTopLevelSpliceError) {
  EXPECT_THROW(Eval("(def xs '(1 2)) `~@xs"), std::runtime_error);
}

// ~x outside any syntax-quote (EvalVisitor Quoted kUnquote branch)
TEST_F(InterpreterTest, UnquoteOutsideSyntaxQuote) {
  EXPECT_EQ(std::get<int_type>(Eval("(def x 7) ~x")), 7);
}

// defmacro with fewer than 3 args (Defmacro::Call arity check)
TEST_F(InterpreterTest, DefmacroTooFewArgs) {
  EXPECT_THROW(Eval("(defmacro name [])"), std::runtime_error);
}

// macroexpand called on a non-macro (Macroexpand::FnCall error branch)
TEST_F(InterpreterTest, MacroexpandNonMacro) {
  EXPECT_THROW(Eval("(macroexpand '(+ 1 2))"), std::runtime_error);
}

// Macro body ends with a user-fn call (tail position → TailCall sentinel).
// The macro expansion site must trampoline to get the actual expanded form.
TEST_F(InterpreterTest, MacroBodyTailCallTrampoline) {
  EXPECT_EQ(std::get<int_type>(
                Eval("(defn make-add [a b] `(+ ~a ~b))"
                     "(defmacro add-macro [a b] (make-add a b))"
                     "(add-macro 3 4)")),
            7);
}

}  // namespace simpl
