// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.h"

#include <ctype.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

#include "simpl/ast.h"
#include "simpl/error.h"
#include "simpl/interpreter.h"
#include "simpl/lexer.h"
#include "simpl/parser.h"

namespace simpl {

Expr run(const std::string &source, Timing *timing) {
  auto t1 = std::chrono::high_resolution_clock::now();
  Lexer lexer(source);
  std::list<Token> tokens = lexer.scan();
  auto t2 = std::chrono::high_resolution_clock::now();
  if (HadError()) {
    throw std::runtime_error("An error occurred while scanning tokens.");
  }
  try {
    Parser parser(tokens);
    auto ast = parser.Parse();
    auto t3 = std::chrono::high_resolution_clock::now();
    static Interpreter interpreter;
    const auto &result = interpreter.Evaluate(ast);
    auto t4 = std::chrono::high_resolution_clock::now();
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
      std::cout << run(line) << std::endl;
    } catch (const std::runtime_error &e) {
      std::cerr << e.what() << std::endl;
      ClearError();
      ClearRuntimeError();
    }
  }
}

}  // namespace simpl
