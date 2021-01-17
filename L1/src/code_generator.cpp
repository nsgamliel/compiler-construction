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
    outputFile << ".text\n.global go\ngo:\npushq %rbx\npushq %rbp\npushq %r12\npushq %r13\npushq %r14\npushq %r15\n";
    outputFile << "call " << "_" << p.entryPointLabel.substr(1) << "\n";

    outputFile << "popq %r15\npopq %r14\npopq %r13\npopq %r12\npopq %rbp\npopq %rbx\nretq\n";
    /* 
     * Close the output file.
     */ 
    outputFile.close();
   
    return ;
  }
}
