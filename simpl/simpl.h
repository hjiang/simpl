// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_SIMPL_H_
#define SIMPL_SIMPL_H_

#include <string>

namespace simpl {

void RunREPL();

void RunFile(const std::string &path);

}  // namespace simpl

#endif  // SIMPL_SIMPL_H_
