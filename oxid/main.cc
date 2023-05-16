#include "oxid.h"

#include <iostream>

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cout << "Usage: " << argv[0] << " [file]" << std::endl;
    return 1;
  } else if (argc == 2) {
    oxid::RunFile(argv[1]);
  } else {
    oxid::RunREPL();
  }

  return 0;
}
