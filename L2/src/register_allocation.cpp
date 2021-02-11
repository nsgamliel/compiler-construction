#include <iostream>

#include <register_allocation.h>
#include <graph_coloring.h>

#define DEGREE_MAX 15

namespace L2 {

	RegisterAllocator::RegisterAllocator(Function* f) {
		f_working = f;
		f_orig = f->clone();
	}

	Function* RegisterAllocator::allocate_registers() {
		this->code_analysis();
		// setup
		for (auto item : f_working->items) {
			auto node = new Node();
			node->var = item;
			// check if gpr
			if (item->is_in(ig->gprs)) {
				node->color = dynamic_cast<Register*>(item);
				if (!(node->color)) std::cout << "faulty cast: " << item->name << std::endl;
				f_working->reduce_v[node->color] = item;
				node->isColored = true;
			}
			nodes.push_back(node);
		}

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
		}

		return f_working;

	}

	void RegisterAllocator::calculate_node_degrees() {
		for (auto node : nodes) {
			if (node->inGraph) {
				// find node's row in interference graph
				for (int i=ig->indices[node->var]*ig->width; i<ig->indices[node->var]*ig->width+ig->width; i++) {
					// increment degree if there is an edge and the connected var is also in the graph
					if (nodes[i - (ig->indices[node->var]*ig->width)]->inGraph && ig->adj_matrix[i]) {
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

}