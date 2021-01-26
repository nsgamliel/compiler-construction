#pragma once

#include <L2.h>

namespace L2{
  L2::Program parse_file (char *fileName);

  L2::Function parse_function_file(char* fileName);
}
