#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <interference.h>
#include <L2.h>

namespace L2 {

	InterferenceGraph::InterferenceGraph(Function* f) {
		// create item vector
		for (auto i : f->instructions) {
			for (auto var : i->gen)
				if (!(var->is_in(this->items))) this->items.push_back(var);
			for (auto var : i->kill)
				if (!(var->is_in(this->items))) this->items.push_back(var);
		}
		for (auto var : this->gprs)
			if (!(var->is_in(this->items))) this->items.push_back(var);
		// create the index mapping
		int index = 0;
		for (auto var : this->items) {
			this->indices[var] = index;
			index++;
		}
		std::cout << "num indices: " << this->indices.size() << std::endl;
		std::cout << "num items: " << this->items.size() << std::endl;
		// create the matrix
		for (int i=0; i<(index*index); i++)
			this->adj_matrix.push_back(false);

		// begin interference analysis

		// connect all gp registers
		for (auto reg1 : this->items) {
			if (reg1->is_in(this->gprs)) {
				for (auto reg2 : this->items) {
					if (reg2->is_in(this->gprs)) {
						if (reg1 != reg2) {
							std::cout << "connect gprs" << std::endl;
							this->add_edge(reg1, reg2);
						}
					}
				}
			}
		}
		// connect INs and OUTs
		for (auto instr : f->instructions) {
			for (auto var1 : instr->in) {
				for (auto var2 : instr->in) {
					if (var1 != var2) {
						std::cout << "connect ins" << std::endl;
						this->add_edge(var1, var2);
					}
				}
			}
			for (auto var1 : instr->out) {
				for (auto var2 : instr->out) {
					if (var1 != var2) {
						std::cout << "connect outs" << std::endl;
						this->add_edge(var1, var2);
					}
				}
			}
		}
		// connect KILLs to OUTs
		for (auto instr : f->instructions) {
			for (auto var1 : instr->kill) {
				for (auto var2 : instr->out) {
					if (var1 != var2) {
						std::cout << "connect kill out" << std::endl;
						this->add_edge(var1, var2);
					}
				}
			}
		}
		// handle target language constraints
		for (auto instr : f->instructions) {
			if (dynamic_cast<Instruction_sop_lsh*> (instr) || dynamic_cast<Instruction_sop_rsh*> (instr)) {
				//instr->gen[0]
				for (auto reg : this->items) {
					if (reg->is_in(this->gprs) && reg->name.compare("rcx") != 0) {
						std::cout << "rcx shift" << std::endl;
						this->add_edge(instr->gen[0], reg);
					}
				}
			}
		}
	}
	
	void InterferenceGraph::add_edge(Variable* start, Variable* end) {
		this->adj_matrix[this->indices[start]*this->indices.size() + this->indices[end]] = true;
		this->adj_matrix[this->indices[end]*this->indices.size() + this->indices[start]] = true;
		std::cout << "num indices: " << this->indices.size() << std::endl;
		std::cout << "num items: " << this->items.size() << std::endl;
	}

	void generate_interference_output(InterferenceGraph* ig) {
		std::cout << "num indices: " << ig->indices.size() << std::endl;
		std::cout << "num items: " << ig->items.size() << std::endl;
		for (int i=0; i<ig->indices.size(); i++) {
			std::cout << "printing name: ";
			std::cout << ig->items[i]->name << std::endl;
			for (int j=0; j<ig->indices.size(); j++) {
				std::cout << "potential interf: " << ig->items[j]->name << std::endl;
				if (i!=j && ig->adj_matrix[ig->indices.size()*i + j])
					std::cout << "yes! " << ig->items[j]->name << std::endl;
			}
			std::cout << "\n";
		}
	}

}