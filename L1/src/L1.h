#pragma once

#include <vector>
#include <string>

namespace L1 {

  enum Register {rdi, rax, rbx, rcx, rdx, rbp, rsi, r8, r9, r10, r11, r12, r13, r14, r15};

  struct Item {
    int type; // 0: register, 1: memory access, 2: const value, else -1
    std::string value; // receives byte offset if memory access or int value if const
    Register r;
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
  struct Instruction_no_var : Instruction{
  };

  struct Instruction_two_var : Instruction{
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
