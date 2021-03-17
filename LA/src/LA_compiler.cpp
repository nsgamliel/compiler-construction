#include <iostream>

#include <LA.h>
#include <parser.h>

int main(int argc, char** argv) {

	bool printM = true;
	// if (printM) std::cout << "" << std::endl;

	if (argc != 2) {
		std::cerr << "error: improper usage" << std::endl;
		return 1;
	}

	LA::Program p;

	if (printM) std::cout << "==== parsing file ====" << std::endl;
	p = LA::parseFile(argv[1]);
	if (printM) std::cout << "==== done parsing ====" << std::endl;
	return 0;

}