#pragma once

#include <string>

#include <interference.h>
#include <spill.h>
#include <L2.h>

namespace L2 {

	struct Node {
		Variable* var;
		Variable* color;
		int degree;
		bool isColored = false;
		bool inGraph = true;
	};

	class RegisterAllocator {
		public:
			RegisterAllocator(Function* f);
			Function* allocate_registers();
			void code_analysis();
			void setup();
			void calculate_node_degrees();
		//private:
			Function* f_working;
			Function* f_orig;
			InterferenceGraph* ig;
			Spiller* sp;
			std::string spill_prefix;
			std::vector<Node*> nodes;
			std::vector<Node*> color_stack;
			std::vector<Node*> nodes_to_spill;
			std::vector<Variable*> callees_to_save;
	};

}