#include <fstream>
#include <iostream>

#include <code_gen.h>

namespace IR {

	bool printCG = false;
	// if (printCG) std::cout << "" << std::endl;

	void generateTargetCode(Program* p) {
		std::ofstream outputFile;
		outputFile.open("prog.L3");

		if (printCG) std::cout << "entering first function" << std::endl;
		for (auto f : p->functions) {
			outputFile << "define " << f->name << " (";
			for (int i=0; i<f->params.size(); i++) {
				outputFile << f->params[i]->name;
				if (i != f->params.size()-1) { outputFile << ", "; }
			}
			outputFile << ") {\n";

			for (auto bb : f->bbsTraced) {
				for (auto instr : bb->instrs) {
					if (printCG) std::cout << "generating instr" << std::endl;
					outputFile << "\t" << instr->toL3String() << "\n";
				}
			}
			outputFile << "\n}\n\n";
		}  

		outputFile.close();
		return;
	}

}