#pragma once

#include <interference.h>
#include <L2.h>

namespace L2 {

	class RegisterAllocator {
		public:
			RegisterAllocator(Function* f);
			Function* allocate_registers();
			void code_analysis();
		//private:
			Function* f_working;
			Function* f_orig;
			InterferenceGraph* ig;
			std::vector<Variable*> spilled_vars;
	};

}