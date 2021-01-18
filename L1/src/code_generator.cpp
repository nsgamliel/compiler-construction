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
   
    /* 
     * Generate target code
     */ 
    outputFile << ".text\n.globl go\ngo:\npushq %rbx\npushq %rbp\npushq %r12\npushq %r13\npushq %r14\npushq %r15\n";
    outputFile << "call " << conv_string(p.entryPointLabel) << "\n";
    outputFile << "popq %r15\npopq %r14\npopq %r13\npopq %r12\npopq %rbp\npopq %rbx\nretq\n";


    for (auto f : p.functions) {
      outputFile << conv_string(f->name) << ":\n";
      for (auto i : f->instructions) {
        switch (i->type) {
          case 0:
            outputFile << "retq\n";
            break;
          case 1:
            outputFile << "movq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
          default:
            outputFile << "# instr placeholder\n";
            break;
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
    //if (item)
    std::string s;
    return s;
  }

  std::string conv_label(const std::string& str) {
    std::string new_str = "_" + str.substr(1);
    return new_str;
  }
}
