// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef OXID_OXID_H_
#define OXID_OXID_H_

#include <string>

namespace oxid {

void RunREPL();

void RunFile(const std::string &path);

}  // namespace oxid

#endif  // OXID_OXID_H_
