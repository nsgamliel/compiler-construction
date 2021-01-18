#pragma once

#include <L1.h>

namespace L1{

  void generate_code(Program p);

  std::string conv_string(const std::string& str);
  std::string conv_operand(const auto& item);

}
