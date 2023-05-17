#ifndef ERROR_H_
#define ERROR_H_

#include <string>

namespace oxid {

void Error(int line, const std::string &message);

void Report(int line, const std::string &where, const std::string &message);

bool HadError();

void ClearError();

}  // namespace oxid

#endif  // ERROR_H_
