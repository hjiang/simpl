#include "oxid.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "error.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"

namespace oxid {

using namespace std;

static void run(const string &source) {
  Lexer lexer(source);
  list<Token> tokens = lexer.scan();
  Parser parser(tokens);
  auto ast = parser.Parse();
  if (HadError()) {
    return;
  }
  Interpreter interpreter;
  try {
    cout << Interpreter::StringifyValue(interpreter.evaluate(ast)) << endl;
  } catch (const std::runtime_error &e) {
    HandleRuntimeError(e);
  }
}

void RunFile(const string &path) {
  cout << "Running file: " << path << endl;
  ifstream file(path);
  if (file) {
    ostringstream ss;
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
    cout << "Could not open file: " << path << endl;
  }
}

void RunREPL() {
  while (true) {
    ClearError();
    cout << "> ";
    string line;
    getline(cin, line);
    if (cin.eof()) {
      break;
    }
    run(line);
  }
}

}  // namespace oxid
