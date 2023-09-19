// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_SIMPL_HH_
#define SIMPL_SIMPL_HH_

#include <chrono>
#include <string>

#include "simpl/ast.hh"

namespace simpl {

struct Timing {
  std::chrono::milliseconds lexer_ms;
  std::chrono::milliseconds parser_ms;
  std::chrono::milliseconds interpreter_ms;
};

Expr run(const std::string &source, Timing *timing = nullptr);
void RunREPL();
void RunFile(const std::string &path);

}  // namespace simpl

#endif  // SIMPL_SIMPL_HH_
