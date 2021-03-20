#include <iostream>

#include <LA.h>
#include <parser.h>
#include <transforms.h>
#include <code_gen.h>

int main(int argc, char** argv) {

	bool printM = false;
	// if (printM) std::cout << "" << std::endl;

	if (argc != 2) {
		std::cerr << "error: improper usage" << std::endl;
		return 1;
	}

	LA::Program p;

	if (printM) std::cout << "==== parsing file ====" << std::endl;
	p = LA::parseFile(argv[1]);
	if (printM) std::cout << "==== done parsing ====" << std::endl;
	if (printM) std::cout << "==== performing code transforms ====" << std::endl;
	for (auto f : p.functions) {
		f->longestVar = "var" + f->longestVar;
		f->longestLabel = "lbl" + f->longestLabel;
		transform(f);
	}
	if (printM) std::cout << "==== done transforming ====" << std::endl;
	if (printM) std::cout << "==== generating code ====" << std::endl;
	generateTargetCode(&p);
	if (printM) std::cout << "==== done code gen ====" << std::endl;
	return 0;

}