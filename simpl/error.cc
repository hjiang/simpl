// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#include "simpl/error.hh"

#include <string>

namespace simpl {

std::string Report(size_t line, const std::string& where,
                   const std::string& message) {
  return "[line " + std::to_string(line) + "] Error " + where + ": " +
         message;
}

[[noreturn]] void Error(size_t line, const std::string& message) {
  throw LexError(Report(line, "", message));
}

}  // namespace simpl

// Local Variables:
// compile-command : "bazel build //simpl:error"
// End:
