#include <algorithm>
#include <iostream>
#include <string>

#include <interference.h>

namespace L2 {

	Function_i* interference_analysis(L2::Function_l* f_l) {
		Function_i* f_i;
		f_i->i_graph = interference_graph_setup(f_l);

		std::vector<size_t> registers_hash;
		for (auto str : f_l->callee_save)
			registers_hash.push_back(f_l->str_hash(str));
		for (auto str : f_l->caller_save)
			registers_hash.push_back(f_l->str_hash(str));

		// check for gp registers
		for (auto item : f_l->items_l) {
			if (std::find(registers_hash.begin(), registers_hash.end(), item.first) != registers_hash.end()) {
				for (auto x : registers_hash) {
					if (item.first != x)
						f_i->i_graph.add_edge(item.first, x);
				}
			}
		}

		// connect everything in IN and OUT sets
		for (auto instr : f_l->instructions) {
			int i;
			int j;
			for (i=0; i<instr->in.size(); i++) {
				for (j=0; j<instr->in.size(); j++) {
					if (i != j)
						f_i->i_graph.add_edge(instr->in[i], instr->in[j]);
				}
			}
			for (i=0; i<instr->out.size(); i++) {
				for (j=0; j<instr->out.size(); j++) {
					if (i != j)
						f_i->i_graph.add_edge(instr->out[i], instr->out[j]);
				}
			}
		}

		// connect everything in kill[i] with everything in out[i]
		for (auto instr : f_l->instructions) {
			int i;
			int j;
			for (i=0; i<instr->kill.size(); i++) {
				for (j=0; j<instr->out.size(); j++) {
					if (instr->kill[i] != instr->out[j])
						f_i->i_graph.add_edge(instr->kill[i], instr->out[j]);
				}
			}
		}

	}

	Interference_graph interference_graph_setup(L2::Function_l* f_l) {
		Interference_graph i_graph;
		std::map<size_t, int> indices;
		std::vector<bool> adj_matrix;
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
		for (int i=0; i<(index_counter*index_counter); i++)
			adj_matrix.push_back(false);

		i_graph.indices = indices;
		i_graph.adj_matrix = adj_matrix;
		return i_graph;
	}

}