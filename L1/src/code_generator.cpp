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
    std::cout << "starting prog.S setup" << std::endl;
    outputFile << "    .text\n    .globl go\ngo:\n    pushq %rbx\n    pushq %rbp\n    pushq %r12\n    pushq %r13\n    pushq %r14\n    pushq %r15\n";
    outputFile << "\n    call " << conv_label(p.entryPointLabel) << "\n\n";
    outputFile << "    popq %r15\n    popq %r14\n    popq %r13\n    popq %r12\n    popq %rbp\n    popq %rbx\n    retq\n\n";
    std::cout << "finished prog.S setup" << std::endl;


    std::cout << "starting function iterations" << std::endl;
    for (auto f : p.functions) {
      std::cout << "in function: " << f->name << std::endl;
      outputFile << conv_label(f->name) << ":\n";
      for (auto i : f->instructions) {
        std::cout << "found instruction: ";
        switch (i->op) {
          case ret:
            outputFile << "    retq\n"; break;
          case mov:
            outputFile << "    movq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case label_def:
            std::cout << "label definition" << std::endl;
            outputFile << "_" << i->items[0]->value.substr(1) << ":\n"; break;
            std::cout << "done label definition" << std::endl;
          default:
            outputFile << "    # instr placeholder\n"; break;
        }
        std::cout << "instruction handled" << std::endl;
      }
      std::cout << "leaving function: " << f->name << std::endl;
    }
    std::cout << "finished function iterations" << std::endl;


    /* 
     * Close the output file.
     */ 
    std::cout << "closing prog.S" << std::endl;
    outputFile.close();
    std::cout << "prog.S closed" << std::endl;
   
    return;
  }

  std::string conv_operand(const Item* item) {
    std::cout << "converting operand" << std::endl;
    std::string s;
    switch (item->type) {
      case 0:
        s = "%" + item->register_name; break;
      case 3:
        s = "$" + conv_label(item->value); break;
    }    
    std::cout << "finished converting operand" << std::endl;
    return s;
  }

  std::string conv_label(const std::string& str) {
    std::cout << "converting label: " << ("_" + str.substr(1)) << std::endl;
    //std::string new_str = "_" + str.substr(1);
    //return new_str;
    //std::cout << "finished converting label" << std::endl;
    return "_" + str.substr(1);
  }
}
