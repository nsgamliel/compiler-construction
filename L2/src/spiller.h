#pragma once

#include <string>

#include <L2.h>

namespace L2 {

	Function_s spill(L2::Function* f, std::string var, std::string prefix);

	void generate_spill_output(Function_s f_s);

}