#include "oxid.h"

#include <fstream>
#include <iostream>
#include <string>

namespace oxid {

using namespace std;

static bool hadError = false;

static void error(int line, const string &message) {
  cout << "[line " << line << "] Error: " << message << endl;
}

static void report(int line, const string &where, const string &message) {
  cout << "[line " << line << "] Error " << where << ": " << message << endl;
  hadError = true;
}

static void run(const string &source) {
  Scanner scanner(source);
  list<Token> tokens = scanner.scanTokens();

  for (Token token : tokens) {
    cout << token << endl;
  }
}

void RunFile(const string &path) {
  cout << "Running file: " << path << endl;
  ifstream file(path);
  if (file) {
    ostringstream ss;
    ss << file.rdbuf();
    run(ss.str());
    if (hadError) {
      exit(65);
    }
  } else {
    cout << "Could not open file: " << path << endl;
  }
}

void RunREPL() {
  while (true) {
    hadError = false;
    cout << "> ";
    string line;
    cin >> line;
    if (cin.eof()) {
      break;
    }
    run(line);
  }
}

} // namespace oxid
