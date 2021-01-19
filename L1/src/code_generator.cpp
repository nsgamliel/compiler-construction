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
    if (item->type) return item; // ie only convert registers (type 0)
    auto new_i = new Item();
    new_i->type = 0;
    switch (item->r) {
      case rcx:
        new_i->register_name = "cl"; break;
    }
    return new_i;
  }
}
