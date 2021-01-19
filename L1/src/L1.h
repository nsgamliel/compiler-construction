#pragma once

#include <vector>
#include <string>

namespace L1 {

  enum Register {rdi, rax, rbx, rcx, rdx, rbp, rsi, r8, r9, r10, r11, r12, r13, r14, r15};

  struct Item {
    int type; // 0: register, 1: memory access, 2: const value, 3: label operand, 4: label defn
    std::string value; // receives byte offset if memory access or int value if const
    std::string register_name;
    Register r;
  };

  /*
   * Instructions.
   */
  enum Opcode {ret, mov, label_def, aop_pe, aop_me, aop_te, aop_ae, aop_pp, aop_mm, sop_lsh, sop_rsh, dir_jmp, cmp_less, cmp_le, cmp_eq, cond_less_jmp};

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
