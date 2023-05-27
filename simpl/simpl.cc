// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/simpl.h"

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

#include "simpl/error.h"
#include "simpl/interpreter.h"
#include "simpl/lexer.h"
#include "simpl/parser.h"

namespace simpl {

static Interpreter::atom_value_type run(const std::string &source) {
  Lexer lexer(source);
  std::list<Token> tokens = lexer.scan();
  Parser parser(tokens);
  auto ast = parser.Parse();
  if (HadError()) {
    return nullptr;
  }
  static Interpreter interpreter;
  try {
    return interpreter.Evaluate(ast);
  } catch (const std::runtime_error &e) {
    HandleRuntimeError(e);
    return nullptr;
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
    std::cout << Interpreter::StringifyValue(run(line)) << std::endl;
  }
}

}  // namespace simpl
