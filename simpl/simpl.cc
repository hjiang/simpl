// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.hh"

#include <ctype.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include "simpl/ast.hh"
#include "simpl/error.hh"
#include "simpl/interpreter.hh"
#include "simpl/lexer.hh"
#include "simpl/parser.hh"
#include "simpl_lib/arrows.hh"

namespace simpl {

namespace {

std::unique_ptr<Interpreter> InitSimpl() {
  std::string source(simpl_lib_arrows_simpl,
                     simpl_lib_arrows_simpl + sizeof(simpl_lib_arrows_simpl));
  Lexer lexer(source);
  auto tokens = lexer.scan();
  Parser parser(tokens);
  auto interpreter = std::make_unique<Interpreter>();
  interpreter->Evaluate(parser.Parse());
  return interpreter;
}

Expr RunInterpreter(Interpreter *interpreter, std::string &&source) {
  Lexer lexer(std::move(source));
  auto tokens = lexer.scan();
  Parser parser(tokens);
  return interpreter->Evaluate(parser.Parse());
}

}  // anonymous namespace

Expr run(const std::string &source, Timing *timing) {
  auto t1 = std::chrono::steady_clock::now();
  Lexer lexer(source);
  std::list<Token> tokens = lexer.scan();
  auto t2 = std::chrono::steady_clock::now();
  if (HadError()) {
    throw std::runtime_error("An error occurred while scanning tokens.");
  }
  try {
    Parser parser(tokens);
    auto ast = parser.Parse();
    auto t3 = std::chrono::steady_clock::now();
    auto interpreter = InitSimpl();
    const auto &result = interpreter->Evaluate(std::move(ast));
    auto t4 = std::chrono::steady_clock::now();
    if (timing) {
      timing->lexer_ms =
          std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
      timing->parser_ms =
          std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2);
      timing->interpreter_ms =
          std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3);
    }
    return result;
  } catch (const Parser::ParseError &e) {
    throw std::runtime_error("An error occurred while parsing the input.");
  } catch (const std::runtime_error &e) {
    HandleRuntimeError(e);
    throw std::runtime_error("An error occurred while evaluating the program.");
  }
}

void RunFile(const std::string &path) {
  std::ifstream file(path);
  if (file) {
    std::ostringstream ss;
    ss << file.rdbuf();
    file.close();
    Timing t;
    run(ss.str(), &t);
    if (HadError()) {
      exit(65);
    }
    if (HadRuntimeError()) {
      exit(70);
    }
    std::cout << "lexer: " << t.lexer_ms.count() << "ms, "
              << "parser: " << t.parser_ms.count() << "ms, "
              << "interpreter: " << t.interpreter_ms.count() << "ms"
              << std::endl;
  } else {
    std::cout << "Could not open file: " << path << std::endl;
  }
}

void RunREPL() {
  std::cout << "Simpl " << kVersion << std::endl;
  auto interpreter = InitSimpl();
  while (true) {
    ClearError();
    std::cout << "> ";
    std::string line;
    getline(std::cin, line);
    if (std::cin.eof()) {
      break;
    }
    if (std::all_of(line.begin(), line.end(), isspace)) {
      continue;
    }
    try {
      std::cout << RunInterpreter(interpreter.get(), std::move(line))
                << std::endl;
    } catch (const std::runtime_error &e) {
      std::cerr << e.what() << std::endl;
      ClearError();
      ClearRuntimeError();
    }
  }
}

}  // namespace simpl
