#include "oxid.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "error.h"
#include "lexer.h"

namespace oxid {

using namespace std;

static void run(const string &source) {
  Lexer lexer(source);
  list<Token> tokens = lexer.scan();

  for (Token token : tokens) {
    cout << string(token) << endl;
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

} // namespace oxid
