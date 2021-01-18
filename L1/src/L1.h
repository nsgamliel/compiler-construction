#pragma once

#include <vector>
#include <string>

namespace L1 {

  //enum Register {rdi, rax, rbx, rcx, rdx, rbp, rsi, r8, r9, r10, r11, r12, r13, r14, r15};

  struct Item {
    int type; // 0: register, 1: memory access, 2: const value, else -1
    std::string value; // receives byte offset if memory access or int value if const
    std::string register_name;
  };

  /*
   * Instructions.
   */
  struct Instruction{
    int type; // consider making this std::string opcode;
    std::vector<Item *> items;
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
