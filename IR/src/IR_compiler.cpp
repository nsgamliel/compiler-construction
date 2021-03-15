#include <iostream>

#include <IR.h>
#include <parser.h>
#include <tracing.h>
#include <translations.h>

int main(int argc, char** argv) {

	bool printM = true;
	// if (printM) std::cout << "" << std::endl;

	if (argc != 2) {
		std::cerr << "error: improper usage" << std::endl;
		return 1;
	}

	IR::Program p;

	if (printM) std::cout << "==== parsing file ====" << std::endl;
	p = IR::parseFile(argv[1]);
	if (printM) std::cout << "==== done parsing ====" << std::endl;
	if (printM) std::cout << "==== tracing functions ====" << std::endl;
	for (auto f : p.functions) {
		traceFunction(f);
	}
	if (printM) std::cout << "==== done tracing ====" << std::endl;
	if (printM) std::cout << "==== translating IR instructions ====" << std::endl;
	for (auto f : p.functions) {
		translate(f);
	}
	if (printM) std::cout << "==== done translating ====" << std::endl;
	return 0;

}