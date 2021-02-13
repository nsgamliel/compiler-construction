#include <iostream>

#include <register_allocation.h>
#include <stack_management.h>

#define DEGREE_MAX 15
#define END_CALLER_SAVE 8

namespace L2 {

	bool printR = false;
	// if (printR) std::cout << "" << std::endl;

	RegisterAllocator::RegisterAllocator(Function* f) {
		f_working = f;
		if (printR) std::cout << "beginning function clone" << std::endl;
		f_orig = f->clone();
		if (printR) std::cout << "clone complete" << std::endl;
		sp = new Spiller();
		spill_prefix = "%SPILLED_VARIABLE_";
	}

	Function* RegisterAllocator::allocate_registers() {
		if (printR) std::cout << "in allocate registers" << std::endl;
		code_analysis();
		setup();

		// graph coloring

		while (!(ig->isColored)) {
			if (printR) std::cout << "top of color loop" << std::endl;
			if (printR) std::cout << "removing nodes" << std::endl;
			node_removal();
			if (printR) std::cout << "nodes removed, coloring" << std::endl;
			color_assignment();
			if (printR) std::cout << "colors assigned" << std::endl;
			
			// check for any vars to be spilled, spilling all
			if (nodes_to_spill.size() > 0) {
				if (printR) std::cout << "spilling nodes: ";
				for (auto node : nodes_to_spill) {
					if (printR) std::cout << node->var->name << " ";
					sp->spill(f_working, node->var, spill_prefix);
				}
				if (printR) std::cout << "" << std::endl;
				if (printR) L2::generate_spill_output(f_working);
				// clear everything, reanalyze, start over
				if (printR) std::cout << "clearing and starting over" << std::endl;
				nodes.clear();
				color_stack.clear();
				nodes_to_spill.clear();
				callees_to_save.clear();
				delete ig;
				code_analysis();
				setup();
				continue;
			}
			// double-check that everything has been colored
			// if not, coloring is impossible, spill all vars and start over
			bool complete_fail = false;
			for (auto node : nodes) {
				if (!(node->isColored)) {
					// spill all vars
					for (auto x : nodes) {
						if (!(x->var->is_in(ig->gprs))) {
							sp->num_replace = 0;
							sp->spill(f_orig, x->var, spill_prefix);
						}
						nodes.clear();
						color_stack.clear();
						nodes_to_spill.clear();
						callees_to_save.clear();
						f_working = f_orig;
						this->code_analysis();
						this->setup();
						complete_fail = true;
						if (printR) std::cout << "complete fail" << std::endl;
					}
				}
			}
			
			if (!complete_fail) ig->isColored = true;

		}

		// assuming we've made it this far, maintain calling convention
		if (callees_to_save.size() > 0) {
			L2::save_to_stack(f_working, callees_to_save);
		}

		replace_colored_nodes();

		return f_working;

	}

	void RegisterAllocator::replace_colored_nodes() {
		for (auto node : nodes) {
			for (auto i : f_working->instructions) {
				i->replace_var(node->var, node->color);
			}
		}
		return;
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
		if (printR) std::cout << "in code analysis" << std::endl;
		f_working->generate_liveness();
		if (printR) std::cout << "liveness generated" << std::endl;
		ig = new InterferenceGraph(f_working);
		if (printR) std::cout << "ig initialized" << std::endl;
		if (printR) generate_interference_output(ig);
		if (printR) std::cout << "finished output" << std::endl;
		return;
	}

	void RegisterAllocator::setup() {
		for (auto item : f_working->items) {
			auto node = new Node();
			node->var = item;
			if (printR) std::cout << "adding node to nodes: " << item->name << std::endl;
			// check if gpr
			if (item->is_in(ig->gprs)) {
				node->color = item;
				node->isColored = true;
			}
			nodes.push_back(node);
		}
		return;
	}

	void RegisterAllocator::node_removal() {
		// calculate expected number of nodes to color
		int to_color = 0;
		for (auto node : nodes) {
			if (!(node->isColored)) {
				to_color++;
			}
		}
		if (printR) std::cout << "number expected to color: " << to_color << std::endl;
		// remove nodes from graph
		while (to_color > 0) {
			calculate_node_degrees();
			int to_pop_ind = -1;
			// determine node to remove
			if (printR) std::cout << "finding winner node" << std::endl;
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
			if (printR) std::cout << "adding node to color stack: " << nodes[to_pop_ind]->var->name << std::endl;
			color_stack.push_back(nodes[to_pop_ind]);
			to_color--;
		}
		return;
	}

	void RegisterAllocator::color_assignment() {
		while (color_stack.size() > 0) {
			auto node = color_stack.back();
			node->inGraph = true;
			// if there are any nodes that have been queued to be spilled, add the rest of the nodes as well without trying to color them
			if (nodes_to_spill.size() > 0) {
				if (node->var->name.substr(0,spill_prefix.length()).compare(spill_prefix) != 0) {
					nodes_to_spill.push_back(node);
				}
				color_stack.pop_back();
				continue;
			}

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
		return;
	}

}