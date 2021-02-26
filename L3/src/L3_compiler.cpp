#include <iostream>

#include <L3.h>
#include <parser.h>
#include <liveness.h>
#include <instruction_selection.h>
#include <code_generation.h>

int main(int argc, char** argv) {

	bool printM = false;
	// if (printM) std::cout << "" << std::endl;

	if (argc != 2) {
		std::cerr << "error: improper usage" << std::endl;
		return 1;
	}

	L3::Program p;
	if (printM) std::cout << "====parsing file: " << argv[1] << std::endl;
	p = L3::parseFile(argv[1]);
	if (printM) std::cout << "====globalizing labels" << std::endl;
	p.globalizeLabels();
	if (printM) std::cout << "====entering individual functions" << std::endl;
	for (auto f : p.functions) {
		if (printM) std::cout << "====generating liveness" << std::endl;
		f->generateLiveness();
		if (printM) std::cout << "====performing instruction selection" << std::endl;
		f->selectTargetInstructions();
		if (printM) std::cout << "====leaving function" << std::endl;
	}
	generateTargetCode(&p);

	if (printM) std::cout << "====done" << std::endl;

	return 0;
}

