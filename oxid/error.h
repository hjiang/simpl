// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef OXID_ERROR_H_
#define OXID_ERROR_H_

#include <stdexcept>
#include <string>

namespace oxid {

void Error(int line, const std::string &message);

void Report(int line, const std::string &where, const std::string &message);

void HandleRuntimeError(const std::runtime_error &error);

bool HadError();
bool HadRuntimeError();

void ClearError();
void ClearRuntimeError();

}  // namespace oxid

#endif  // OXID_ERROR_H_
