#include "error.h"

#include <iostream>
#include <string>

namespace oxid {

static bool gHadError = false;

void Error(int line, const std::string &message) { Report(line, "", message); }

void Report(int line, const std::string &where, const std::string &message) {
  std::cout << "[line " << line << "] Error " << where << ": " << message
            << std::endl;
  gHadError = true;
}

bool HadError() { return gHadError; }

void ClearError() { gHadError = false; }

}  // namespace oxid

// Local Variables:
// compile-command : "bazel build //oxid:error"
// End:
