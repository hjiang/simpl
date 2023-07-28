// Copyright 2023 Hong Jiang <lazyseq@gmail.com> and the contributors

#ifndef SIMPL_ERROR_H_
#define SIMPL_ERROR_H_

#include <stdexcept>
#include <string>

namespace simpl {

void Error(size_t line, const std::string &message);

void Report(size_t line, const std::string &where, const std::string &message);

void HandleRuntimeError(const std::runtime_error &error);

bool HadError();
bool HadRuntimeError();

void ClearError();
void ClearRuntimeError();

}  // namespace simpl

#endif  // SIMPL_ERROR_H_
