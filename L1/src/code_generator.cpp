#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;


namespace L1{

  bool printGActions = true;

  void generate_code(Program p){
    /* 
     * Open the output file.
     */ 
    if (printGActions) std::cout << "opening prog.S..." << std::endl;
    std::ofstream outputFile;
    outputFile.open("prog.S");
    if (printGActions) std::cout << "prog.S opened" << std::endl;
   
    /* 
     * Generate target code
     */ 
    if (printGActions) std::cout << "constructing base template..." << std::endl;
    outputFile << "    .text\n    .globl go\ngo:\n    pushq %rbx\n    pushq %rbp\n    pushq %r12\n    pushq %r13\n    pushq %r14\n    pushq %r15\n";
    outputFile << "\n    call " << conv_label(p.entryPointLabel) << "\n\n";
    outputFile << "    popq %r15\n    popq %r14\n    popq %r13\n    popq %r12\n    popq %rbp\n    popq %rbx\n    retq\n\n";
    if (printGActions) std::cout << "base template constructed" << std::endl;


    if (printGActions) std::cout << "entering functions..." << std::endl;
    for (auto f : p.functions) {
      if (printGActions) std::cout << "found function " << f->name << std::endl;
      outputFile << conv_label(f->name) << ":\n";
      if (printGActions) std::cout << "entering instructions..." << std::endl;
      for (auto i : f->instructions) {
        if (printGActions) std::cout << "found ";
        switch (i->op) {
          case ret:
            if (printGActions) std::cout << "return instruction" << std::endl;
            outputFile << "    retq\n"; break;
          case mov:
            if (printGActions) std::cout << "move instruction" << std::endl;
            outputFile << "    movq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case label_def:
            if (printGActions) std::cout << "label def instruction" << std::endl;
            outputFile << conv_operand(i->items[0]) << "\n"; break;
          case aop_pe:
            if (printGActions) std::cout << "aop_pe instruction" << std::endl;
            outputFile << "    addq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_me:
            if (printGActions) std::cout << "aop_me instruction" << std::endl;
            outputFile << "    subq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_te:
            if (printGActions) std::cout << "aop_te instruction" << std::endl;
            outputFile << "    imulq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_ae:
            if (printGActions) std::cout << "aop_ae instruction" << std::endl;
            outputFile << "    andq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_pp:
            if (printGActions) std::cout << "aop_pp instruction" << std::endl;
            outputFile << "    inc " << conv_operand(i->items[0]) << "\n"; break;
          case aop_mm:
            if (printGActions) std::cout << "aop_mm instruction" << std::endl;
            outputFile << "    dec " << conv_operand(i->items[0]) << "\n"; break;
          case sop_lsh:
            if (printGActions) std::cout << "sop_lsh instruction" << std::endl;
            outputFile << "    salq " << conv_operand(to_8_bit(i->items[0])) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case sop_rsh:
            if (printGActions) std::cout << "sop_rsh instruction" << std::endl;
            outputFile << "    sarq " << conv_operand(to_8_bit(i->items[0])) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case dir_jmp:
            if (printGActions) std::cout << "dir_jmp instruction" << std::endl;
            outputFile << "    jmp " << conv_label(i->items[0]->value) << "\n"; break;
          case cmp_less:
            if (printGActions) std::cout << "cmp_less instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              outputFile << "    movq $" << int(std::stoi(i->items[1]->value) < std::stoi(i->items[0]->value)) << ", " << conv_operand(i->items[2]) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    setg " << conv_operand(to_8_bit(i->items[2])) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    setl " << conv_operand(to_8_bit(i->items[2])) << "\n";
              }
              outputFile << "    movzbq " << conv_operand(to_8_bit(i->items[2])) << ", " << conv_operand(i->items[2]) << "\n";
            } break;
          case cmp_le:
            if (printGActions) std::cout << "cmp_le instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              outputFile << "    movq $" << int(std::stoi(i->items[1]->value) <= std::stoi(i->items[0]->value)) << ", " << conv_operand(i->items[2]) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    setge " << conv_operand(to_8_bit(i->items[2])) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    setle " << conv_operand(to_8_bit(i->items[2])) << "\n";
              }
              outputFile << "    movzbq " << conv_operand(to_8_bit(i->items[2])) << ", " << conv_operand(i->items[2]) << "\n";
            } break;
          case cmp_eq:
            if (printGActions) std::cout << "cmp_eq instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              outputFile << "    movq $" << int(std::stoi(i->items[1]->value) == std::stoi(i->items[0]->value)) << ", " << conv_operand(i->items[2]) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
              }
              outputFile << "    sete " << conv_operand(to_8_bit(i->items[2])) << "\n";
              outputFile << "    movzbq " << conv_operand(to_8_bit(i->items[2])) << ", " << conv_operand(i->items[2]) << "\n";
            } break;
          case cond_less_jmp:
            if (printGActions) std::cout << "cond_less_jmp instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              if (int(std::stoi(i->items[1]->value) < std::stoi(i->items[0]->value)))
                outputFile << "    jmp " << conv_label(i->items[2]->value) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    jg " << conv_label(i->items[2]->value) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    jl " << conv_label(i->items[2]->value) << "\n";
              }
            } break;
          case cond_le_jmp:
            if (printGActions) std::cout << "cond_le_jmp instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              if (int(std::stoi(i->items[1]->value) <= std::stoi(i->items[0]->value)))
                outputFile << "    jmp " << conv_label(i->items[2]->value) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    jge " << conv_label(i->items[2]->value) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    jle " << conv_label(i->items[2]->value) << "\n";
              }
            } break;
          case cond_eq_jmp:
            if (printGActions) std::cout << "cond_eq_jmp instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              if (int(std::stoi(i->items[1]->value) == std::stoi(i->items[0]->value)))
                outputFile << "    jmp " << conv_label(i->items[2]->value) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
              }
              outputFile << "    je " << conv_label(i->items[2]->value) << "\n";
            } break;
          default:
            if (printGActions) std::cout << "unknown instruction" << std::endl;
            outputFile << "    # instr placeholder\n"; break;
        }
      }
    }


    /* 
     * Close the output file.
     */ 
    if (printGActions) std::cout << "closing prog.S..." << std::endl;
    outputFile.close();
    if (printGActions) std::cout << "prog.S closed" << std::endl;
   
    return;
  }

  std::string conv_operand(const Item* item) {
    if (printGActions) std::cout << "converting operand ";
    std::string s;
    switch (item->type) {
      case 0:
        if (printGActions) std::cout << "register " << item->register_name << std::endl;
        s = "%" + item->register_name; break;
      case 1:
        if (printGActions) std::cout << "mem access " << item->register_name << " " << item->value << std::endl;
        s = item->value + "(%" + item->register_name + ")"; break;
      case 2:
        if (printGActions) std::cout << "constant " << item->value << std::endl;
        s = "$" + item->value; break;
      case 3:
        if (printGActions) std::cout << "label " << item->value << std::endl;
        s = "$" + conv_label(item->value); break;
      case 4:
        if (printGActions) std::cout << "label definition " << item->value << std::endl;
        s = "    " + conv_label(item->value) + ":"; break;
    }
    if (printGActions) std::cout << "done converting operand" << std::endl;
    return s;
  }

  std::string conv_label(const std::string& str) {
    if (printGActions) std::cout << "converting label " << str << std::endl;
    return "_" + str.substr(1);
  }

  Item* to_8_bit(Item* item) {
    if (printGActions) std::cout << "in to_8_bit" << std::endl;
    if (item->type) return item; // ie only convert registers (type 0)
    if (printGActions) std::cout << "need to convert" << std::endl;
    auto new_i = new Item();
    new_i->type = 0;
    switch (item->r) {
      case rax:
        if (printGActions) std::cout << "8-bit rax" << std::endl;
        new_i->register_name = "al"; break;
      case rbx:
        if (printGActions) std::cout << "8-bit rbx" << std::endl;
        new_i->register_name = "bl"; break;
      case rcx:
        if (printGActions) std::cout << "8-bit rcx" << std::endl;
        new_i->register_name = "cl"; break;
      case rdx:
        if (printGActions) std::cout << "8-bit rdx" << std::endl;
        new_i->register_name = "dl"; break;
      case rdi:
        if (printGActions) std::cout << "8-bit rdi" << std::endl;
        new_i->register_name = "dil"; break;
      case rbp:
        if (printGActions) std::cout << "8-bit rbp" << std::endl;
        new_i->register_name = "bpl"; break;
      case rsi:
        if (printGActions) std::cout << "8-bit rsi" << std::endl;
        new_i->register_name = "sil"; break;
      case r8:
        if (printGActions) std::cout << "8-bit r8" << std::endl;
        new_i->register_name = "r8b"; break;
      case r9:
        if (printGActions) std::cout << "8-bit r9" << std::endl;
        new_i->register_name = "r9b"; break;
      case r10:
        if (printGActions) std::cout << "8-bit r10" << std::endl;
        new_i->register_name = "r10b"; break;
      case r11:
        if (printGActions) std::cout << "8-bit r11" << std::endl;
        new_i->register_name = "r11b"; break;
      case r12:
        if (printGActions) std::cout << "8-bit r12" << std::endl;
        new_i->register_name = "r12b"; break;
      case r13:
        if (printGActions) std::cout << "8-bit r13" << std::endl;
        new_i->register_name = "r13b"; break;
      case r14:
        if (printGActions) std::cout << "8-bit r14" << std::endl;
        new_i->register_name = "r14b"; break;
      case r15:
        if (printGActions) std::cout << "8-bit r15" << std::endl;
        new_i->register_name = "r15b"; break;
    }
    if (printGActions) std::cout << "new register name " << new_i->register_name << std::endl;
    return new_i;
  }
}
