#pragma once

#include <map>
#include <vector>

#include <L2.h>

namespace L2 {

	class InterferenceGraph {
		public:
			InterferenceGraph(Function* f);
			//helpers
			void add_edge(Variable* start, Variable* end);

		//private:
			Function* funct;
			std::map<Variable*, int> indices;
			std::vector<bool> adj_matrix;
			bool isColored;

			std::vector<L2::Variable*> gprs = {new Register("r12", r12), new Register("r13", r13), new Register("r14", r14), new Register("r15", r15), new Register("rbp", rbp), new Register("rbx", rbx), new Register("rdi", rdi), new Register("rsi", rsi), new Register("rdx", rdx), new Register("rcx", rcx), new Register("r8", r8), new Register("r9", r9), new Register("r10", r10), new Register("r11", r11), new Register("rax", rax)};
	};

	void generate_interference_output(InterferenceGraph* ig);

}