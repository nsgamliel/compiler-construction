#pragma once

#include <vector>
#include <string>

namespace L1 {

  enum Register {rdi, rax, rbx, rcx, rdx, rbp, rsi, r8, r9, r10, r11, r12, r13, r14, r15};

  struct Item {
    std::string labelName;
    Register r;
    bool isARegister;
  };

  /*
   * Instruction interface.
   */
  struct Instruction{
    int type;
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
