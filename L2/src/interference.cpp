#include <iostream>

#include <interference.h>

namespace L2 {

	Function_i* interference_analysis(L2::Function_l* f_l) {
		Function_i* f_i;
		f_i->i_graph = interference_graph_setup(f_l);

		// check for gp registers
		// 
	}

	Interference_graph interference_graph_setup(L2::Function_l* f_l) {
		Interference_graph i_graph;
		std::map<size_t, int> indices;
		bool* adj_matrix;
		int index_counter = 0;

		// add all items
		for (auto item : f_l->items_l) {
			if (indices.find(item.first) == indices.end()) { // not here
				indices[item.first] = index_counter;
				index_counter++;
			}
		}
		// add any unused callee-save registers
		for (auto str : f_l->callee_save) {
			if (indices.find(f_l->str_hash(str)) == indices.end()) {
				indices[f_l->str_hash(str)] = index_counter;
				index_counter++;
			}
		}
		// add any unused caller-save registers
		for (auto str : f_l->caller_save) {
			if (indices.find(f_l->str_hash(str)) == indices.end()) {
				indices[f_l->str_hash(str)] = index_counter;
				index_counter++;
			}
		}
		// create the matrix
		adj_matrix = new bool[index_counter * index_counter];
		for (int i=0; i<adj_matrix.size(); i++)
			adj_matrix[i] = false;

		i_graph.indices = indices;
		i_graph.adj_matrix = adj_matrix;
		return i_graph;
	}

}