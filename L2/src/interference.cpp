#include <interference.h>
#include <L2.h>

namespace L2 {

	InterferenceGraph::add_edge(Variable* start, Variable* end) {
		this->adj_matrix[this->indices[start]*this->indices.size() + this->indices[end]] = true;
		this->adj_matrix[this->indices[end]*this->indices.size() + this->indices[start]] = true;
	}

}