#pragma once

#include <string>

#include <L2.h>

namespace L2 {

	struct Function_s {
		std::string name;
	    int64_t arguments;
	    int64_t locals = 0;
	    int num_replace = 0;
	    std::vector<L2::Instruction *> instructions;
	};

	Function_s spill(L2::Function* f, std::string var, std::string prefix);

	void generate_spill_output(Function_s f_s);

}