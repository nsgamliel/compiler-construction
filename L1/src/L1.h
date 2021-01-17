#pragma once

#include <vector>
#include <string>

namespace L1 {

  enum Register {rdi, rax};

  struct Item {
    std::string labelName;
    Register r;
    bool isARegister;
  };

  /*
   * Instruction interface.
   */
  struct Instruction{
  };

  /*
   * Instructions.
   */
  struct Instruction_ret : Instruction{
  };

  struct Instruction_assignment : Instruction{
    Item src,dst;
  };

  /*
   * Function.
   */
  struct Function{
    std::string name;
    int64_t arguments;
    int64_t locals;
    std::vector<Instruction *> instructions;
  };

  /*
   * Program.
   */
  struct Program{
    std::string entryPointLabel;
    std::vector<Function *> functions;
  };

}
