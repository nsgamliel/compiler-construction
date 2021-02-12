#include <iostream>

#include <register_allocation.h>

#define DEGREE_MAX 15
#define END_CALLER_SAVE 8

namespace L2 {

	RegisterAllocator::RegisterAllocator(Function* f) {
		f_working = f;
		f_orig = f->clone();
		sp = new Spiller();
		spill_prefix = "SPILLED_VARIABLE_";
	}

	Function* RegisterAllocator::allocate_registers() {
		this->code_analysis();
		this->setup();

		// graph coloring

		while (!(ig->isColored)) {
			// calculate expected number of nodes to color
			int to_color = 0;
			for (auto node : nodes) {
				if (!(node->isColored)) {
					to_color++;
				}
			}
			// remove nodes from graph
			while (to_color > 0) {
				calculate_node_degrees();
				int to_pop_ind = -1;
				// determine node to remove
				for (int i=0; i<nodes.size(); i++) {
					if (!(nodes[i]->isColored) && nodes[i]->inGraph && nodes[i]->degree < DEGREE_MAX) {
						if (to_pop_ind == -1 || nodes[i]->degree > nodes[to_pop_ind]->degree) {
							to_pop_ind = i;
						}
					}
				}
				// if to_pop_ind is still -1, all nodes of degree < DEGREE_MAX have been removed
				// proceed with removing nodes of degree > DEGREE_MAX
				if (to_pop_ind == -1) {
					for (int i=0; i<nodes.size(); i++) {
						if (!(nodes[i]->isColored) && nodes[i]->inGraph) {
							if (to_pop_ind == -1 || nodes[i]->degree > nodes[to_pop_ind]->degree) {
								to_pop_ind = i;
							}
						}
					}
				}
				if (to_pop_ind == -1) std::cerr << "error in node removal" << std::endl;
				nodes[to_pop_ind]->inGraph = false;
				color_stack.push_back(nodes[to_pop_ind]);
				to_color--;
			}
			// assign colors
			while (color_stack.size() > 0) {
				auto node = color_stack.back();
				node->inGraph = true;
				// iterate through possible registers, checking if any edges lead to the given color
				int base = ig->indices[f_working->reduce_v[node->var]]*ig->width;
				for (int reg=0; reg<ig->gprs.size(); reg++) {
					bool color_candidate = true;
					// examine all edges connected to the given node that are currently present in the graph
					for (int i=base; i<base+ig->width; i++) {
						// if an edge is present and it is both currently in the graph and colored
						if (ig->adj_matrix[i] && nodes[i - base]->inGraph && nodes[i - base]->isColored) {
							// if connected node's color matches the current one, move on
							if (f_working->reduce_v[nodes[i - base]->color] == f_working->reduce_v[ig->gprs[reg]]) {
								color_candidate = false;
								break;
							}
						}
					}
					if (color_candidate) {
						node->color = f_working->reduce_v[ig->gprs[reg]];
						node->isColored = true;
						if (reg > END_CALLER_SAVE) {
							callees_to_save.push_back(ig->gprs[reg]);
						}
						break;
					}
				}
				// if the node was not successfully colored and it is not a previously spilled var, 
				// add it to vector of nodes to be spilled and add node back to graph
				if (!(node->isColored) && node->var->name.substr(0,spill_prefix.length()).compare(spill_prefix) != 0) {
					nodes_to_spill.push_back(node);
				}
				color_stack.pop_back();
			}
			// check for any vars to be spilled, spilling all
			if (nodes_to_spill.size() > 0) {
				for (auto node : nodes_to_spill) {
					sp->spill(f_working, node->var, spill_prefix);
				}
				// clear everything, reanalyze, start over
				nodes.clear();
				color_stack.clear();
				nodes_to_spill.clear();
				callees_to_save.clear();
				this->code_analysis();
				this->setup();
				continue;
			}
			// double-check that everything has been colored
			// if not, coloring is impossible, spill all vars and start over
			for (auto node : nodes) {
				if (!(node->isColored)) {
					// spill all vars
					for (auto x : nodes) {
						if (!(x->var->is_in(ig->gprs))) {
							sp->num_replace = 0;
							sp->spill(f_orig, x->var, spill_prefix);
						}
						return f_orig;
					}
				}
			}
			
			ig->isColored = true;

		}

		return f_working;

	}

	void RegisterAllocator::calculate_node_degrees() {
		for (auto node : nodes) {
			if (node->inGraph) {
				// find node's row in interference graph
				int base = ig->indices[f_working->reduce_v[node->var]]*ig->width;
				for (int i=base; i<base+ig->width; i++) {
					// increment degree if there is an edge and the connected var is also in the graph
					if (ig->adj_matrix[i] && nodes[i - base]->inGraph) {
						node->degree++;
					}
				}
			} else {
				node->degree = 0;
			}
		}
		return;
	}

	void RegisterAllocator::code_analysis() {
		f_working->generate_liveness();
		ig = new InterferenceGraph(f_working);
		return;
	}

	void RegisterAllocator::setup() {
		for (auto item : f_working->items) {
			auto node = new Node();
			node->var = item;
			// check if gpr
			if (item->is_in(ig->gprs)) {
				node->color = item;
				node->isColored = true;
			}
			nodes.push_back(node);
		}
		return;
	}

}