#include <map>
#include <vector>

#include <L2.h>

namespace L2 {

	class InterferenceGraph {
		public:
			InterferenceGraph();
			void add_edge(Variable* start, Variable* end);

		//private:
			std::map<Variable*, int> indices;
			std::vector<bool> adj_matrix;
	};

}