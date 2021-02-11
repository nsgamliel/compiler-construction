#pragma once

#include <interference.h>
#include <L2.h>

namespace L2 {

	struct Node {
		Variable* var;
		Register* color;
		int degree;
		bool isColored = false;
		bool inGraph = true;
	};

	class RegisterAllocator {
		public:
			RegisterAllocator(Function* f);
			Function* allocate_registers();
			void code_analysis();
			void calculate_node_degrees();
		//private:
			Function* f_working;
			Function* f_orig;
			InterferenceGraph* ig;
			std::vector<Variable*> spilled_vars;
			std::vector<Node*> nodes;
			std::vector<Node*> color_stack;
	};

}