#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;

namespace L1{
  void generate_code(Program p){

    /* 
     * Open the output file.
     */ 
    std::ofstream outputFile;
    outputFile.open("prog.S");
    std::cout << "created prog.S" << std::endl;
   
    /* 
     * Generate target code
     */ 
    outputFile << "    .text\n    .globl go\ngo:\n    pushq %rbx\n    pushq %rbp\n    pushq %r12\n    pushq %r13\n    pushq %r14\n    pushq %r15\n";
    outputFile << "\n    call " << conv_label(p.entryPointLabel) << "\n\n";
    outputFile << "    popq %r15\n    popq %r14\n    popq %r13\n    popq %r12\n    popq %rbp\n    popq %rbx\n    retq\n\n";


    for (auto f : p.functions) {
      outputFile << conv_label(f->name) << ":\n";
      for (auto i : f->instructions) {
        switch (i->op) {
          case ret:
            outputFile << "    retq\n"; break;
          case mov:
            outputFile << "    movq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case label_def:
            outputFile << conv_label(i->items[0]->value) << ":\n"; break;
          default:
            outputFile << "    # instr placeholder\n"; break;
        }
      }
    }


    /* 
     * Close the output file.
     */ 
    outputFile.close();
   
    return;
  }

  std::string conv_operand(const Item* item) {
    std::string s;
    switch (item->type) {
      case 0:
        s = "%" + item->register_name; break;
      case 3:
        s = "$" + conv_label(item->value); break;
    }
    return s;
  }

  std::string conv_label(const std::string& str) {
    std::string new_str = "_" + str.substr(1);
    return new_str;
  }
}
