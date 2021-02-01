#pragma once

#include <map>
#include <string>
#include <vector>

#include <L2.h>
#include <liveness.h>

namespace L2 {

	struct Interference_graph {
		std::vector<size_t> hashes;
		std::map<size_t, int> indices;
		std::vector<bool> adj_matrix;

		void add_edge(size_t start, size_t end) {
			this->adj_matrix[this->indices[start]*this->indices.size() + this->indices[end]] = true;
			this->adj_matrix[this->indices[end]*this->indices.size() + this->indices[start]] = true;
		}
	};

	struct Function_i {
		Interference_graph i_graph;
		std::map<size_t, std::string> items_i;
	};

	// main entry point for interference analysis
	Function_i* interference_analysis(L2::Function_l* f_l);

	void generate_interference_output(Function_i* f_i);

	// helper function
	Interference_graph interference_graph_setup(L2::Function_l* f_l);

}

/* interference plan

takes in a function that has had liveness analysis performed on it (Function_l)

produces a Function_i (function with interference analysis performed on it)

problem: there are likely to be registers included that aren't in the function which would throw the representation off

add everything already used in the function
	- map each size_t to an integer index in the matrix (that map will live in the Function_i struct)
check each of the registers in caller_save and callee_save (in f_l) and if the hash is not already in the map, add it
then start interference analysis


*/