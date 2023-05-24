// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "oxid/error.h"

#include <iostream>
#include <string>

namespace oxid {

static bool gHadError = false;
static bool gHadRuntimeError = false;

void Error(int line, const std::string &message) { Report(line, "", message); }

void HandleRuntimeError(const std::runtime_error &error) {
  std::cout << error.what() << std::endl;
  gHadRuntimeError = true;
}

void Report(int line, const std::string &where, const std::string &message) {
  std::cout << "[line " << line << "] Error " << where << ": " << message
            << std::endl;
  gHadError = true;
}

bool HadError() { return gHadError; }
bool HadRuntimeError() { return gHadRuntimeError; }

void ClearError() { gHadError = false; }
void ClearRuntimeError() { gHadRuntimeError = false; }

}  // namespace oxid

// Local Variables:
// compile-command : "bazel build //oxid:error"
// End:
