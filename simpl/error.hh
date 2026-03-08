// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_ERROR_HH_
#define SIMPL_ERROR_HH_

#include <stdexcept>
#include <string>

namespace simpl {

class LexError : public std::runtime_error {
 public:
  explicit LexError(const std::string& msg) : std::runtime_error(msg) {}
};

// Returns formatted "[line N] Error<where>: <message>" string (no side effects)
std::string Report(size_t line, const std::string& where,
                   const std::string& message);

// Calls Report() and throws LexError — never returns
[[noreturn]] void Error(size_t line, const std::string& message);

}  // namespace simpl

#endif  // SIMPL_ERROR_HH_
