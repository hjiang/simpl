// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "oxid/oxid.h"

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

#include "oxid/error.h"
#include "oxid/interpreter.h"
#include "oxid/lexer.h"
#include "oxid/parser.h"

namespace oxid {

static void run(const string &source) {
  Lexer lexer(source);
  std::list<Token> tokens = lexer.scan();
  Parser parser(tokens);
  auto ast = parser.Parse();
  if (HadError()) {
    return;
  }
  static Interpreter interpreter;
  try {
    cout << Interpreter::StringifyValue(interpreter.evaluate(ast)) << endl;
  } catch (const std::runtime_error &e) {
    HandleRuntimeError(e);
  }
}

void RunFile(const std::string &path) {
  std::cout << "Running file: " << path << std::endl;
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
  while (true) {
    ClearError();
    std::cout << "> ";
    std::string line;
    getline(std::cin, line);
    if (std::cin.eof()) {
      break;
    }
    run(line);
  }
}

}  // namespace oxid
