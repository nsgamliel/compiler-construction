#include <fstream>

#include <code_generation.h>
#include <tiler.h>

namespace L3 {

	void generateTargetCode(Program* p) {
		std::ofstream outputFile;
		outputFile.open("prog.L2");

		outputFile << "(:main\n";

		std::vector<std::string> regArgs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
		for (auto f : p->functions) {
			outputFile << "\t(" << f->name << " " << f->params.size() << "\n";
			for (int i=0; i<f->params.size(); i++) {
				if (i<6) {
					outputFile << "\t\t" << f->params[i]->name << " <- " << regArgs[i] << "\n";
				} else {
					outputFile << "\t\t" << f->params[i]->name << " <- stack-arg " << (f->params.size()-1-i) * 8 << "\n";
				}
			}
			for (auto t : f->tiles) {
				outputFile << t->generateTargetInstructions();
			}
			outputFile << "\t)\n";
		}  

		outputFile << ")\n";
		outputFile.close();
		return;
	}

}