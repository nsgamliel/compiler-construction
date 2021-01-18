#pragma once

#include <vector>
#include <string>

namespace L1 {

  //enum Register {rdi, rax, rbx, rcx, rdx, rbp, rsi, r8, r9, r10, r11, r12, r13, r14, r15};

  struct Item {
    int type; // 0: register, 1: memory access, 2: const value, 3: label operand, else (label defn) -1
    std::string value; // receives byte offset if memory access or int value if const
    std::string register_name;
  };

  /*
   * Instructions.
   */
  enum Opcode {ret, mov, label_def};

  struct Instruction{
    Opcode op; // figured i'd use this instead of ints since there will be quite a few
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
