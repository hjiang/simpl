// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.h"

#include <ctype.h>

#include <algorithm>
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

static Expr run(const std::string &source) {
  Lexer lexer(source);
  std::list<Token> tokens = lexer.scan();
  if (HadError()) {
    throw std::runtime_error("An error occurred while scanning tokens.");
  }
  try {
    Parser parser(tokens);
    auto ast = parser.Parse();
    static Interpreter interpreter;
    return interpreter.Evaluate(ast);
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
    run(ss.str());
    if (HadError()) {
      exit(65);
    }
    if (HadRuntimeError()) {
      exit(70);
    }
  } else {
    std::cout << "Could not open file: " << path << std::endl;
  }
}

void RunREPL() {
  std::cout << "SimpL " << kVersion << std::endl;
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
      std::cout << Interpreter::StringifyValue(run(line)) << std::endl;
    } catch (const std::runtime_error &e) {
      std::cerr << e.what() << std::endl;
      ClearError();
      ClearRuntimeError();
    }
  }
}

}  // namespace simpl
