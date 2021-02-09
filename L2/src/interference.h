#include <map>
#include <vector>

#include <L2.h>

namespace L2 {

	class InterferenceGraph {
		public:
			InterferenceGraph(Function* f);
			//helpers
			void interference_analysis();
			void add_edge(Variable* start, Variable* end);

		//private:
			std::map<Variable*, int> indices;
			std::vector<bool> adj_matrix;
			std::vector<Variable*> items;

			std::vector<L2::Variable*> gprs = {new Variable("r12"), new Variable("r13"), new Variable("r14"), new Variable("r15"), new Variable("rbp"), new Variable("rbx"), new Variable("rdi"), new Variable("rsi"), new Variable("rdx"), new Variable("rcx"), new Variable("r8"), new Variable("r9"), new Variable("r10"), new Variable("r11"), new Variable("rax")};
	};

	void generate_interference_output(InterferenceGraph* ig);

}