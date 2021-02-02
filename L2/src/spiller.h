#pragma once

#include <string>

#include <L2.h>

namespace L2 {

	L2::Function spill(L2::Function* f, std::string var, std::string prefix);

	void generate_spill_output(L2::Function f_s);

}