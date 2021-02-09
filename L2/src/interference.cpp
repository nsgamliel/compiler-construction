#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

#include <interference.h>
#include <L2.h>

namespace L2 {

	bool printI = false;

	InterferenceGraph::InterferenceGraph(Function* f) {
		this->funct = f;
		// create item vector
		if (printI) std::cout << "creating item and reduce" << std::endl;
		if (printI) std::cout << "adding gprs" << std::endl;
		for (auto var : this->gprs) {
			if (!(var->is_in(f->items))) {
				f->items.push_back(var);
				f->reduce_v[var] = var;
			} else f->reduce_v[var] = var->get_dup(f->items);
		}
		if (printI) std::cout << "entering instructions: " << f->instructions.size() << std::endl;
		for (auto i : f->instructions) {
			if (printI) std::cout << "adding gens" << std::endl;
			for (auto var : i->gen) {
				if (!(var->is_in(f->items))) {
					f->items.push_back(var);
					f->reduce_v[var] = var;
				} else f->reduce_v[var] = var->get_dup(f->items);
			}
			if (printI) std::cout << "adding kills" << std::endl;
			for (auto var : i->kill) {
				if (!(var->is_in(f->items))) {
					f->items.push_back(var);
					f->reduce_v[var] = var;
				} else f->reduce_v[var] = var->get_dup(f->items);
			}
			if (printI) std::cout << "adding ins" << std::endl;
			for (auto var : i->in) {
				if (!(var->is_in(f->items))) {
					f->items.push_back(var);
					f->reduce_v[var] = var;
				} else f->reduce_v[var] = var->get_dup(f->items);
			}
			if (printI) std::cout << "adding outs" << std::endl;
			for (auto var : i->out) {
				if (!(var->is_in(f->items))) {
					f->items.push_back(var);
					f->reduce_v[var] = var;
				} else f->reduce_v[var] = var->get_dup(f->items);
			}
		}
		if (printI) std::cout << "done" << std::endl;
		// create the index mapping
		int index = 0;
		for (auto var : f->items) {
			this->indices[var] = index;
			index++;
		}
		if (printI) std::cout << "num indices: " << this->indices.size() << std::endl;
		if (printI) std::cout << "num items: " << f->items.size() << std::endl;
		// create the matrix
		for (int i=0; i<(index*index); i++)
			this->adj_matrix.push_back(false);

		// begin interference analysis

		// connect all gp registers
		for (auto reg1 : this->gprs) {
			for (auto reg2 : this->gprs) {
				if (f->reduce_v[reg1] != f->reduce_v[reg2]) {
					if (printI) std::cout << "connect gprs" << std::endl;
					this->add_edge(f->reduce_v[reg1], f->reduce_v[reg2]);
				}
			}
		}
		// connect INs and OUTs
		for (auto instr : f->instructions) {
			for (auto var1 : instr->in) {
				for (auto var2 : instr->in) {
					if (f->reduce_v[var1] != f->reduce_v[var2]) {
						if (printI) std::cout << "connect ins" << std::endl;
						this->add_edge(f->reduce_v[var1], f->reduce_v[var2]);
					}
				}
			}
			for (auto var1 : instr->out) {
				for (auto var2 : instr->out) {
					if (f->reduce_v[var1] != f->reduce_v[var2]) {
						if (printI) std::cout << "connect outs" << std::endl;
						this->add_edge(f->reduce_v[var1], f->reduce_v[var2]);
					}
				}
			}
		}
		// connect KILLs to OUTs
		for (auto instr : f->instructions) {
			for (auto var1 : instr->kill) {
				for (auto var2 : instr->out) {
					if (f->reduce_v[var1] != f->reduce_v[var2]) {
						if (printI) std::cout << "connect kill out" << std::endl;
						this->add_edge(f->reduce_v[var1], f->reduce_v[var2]);
					}
				}
			}
		}
		// handle target language constraints
		for (auto instr : f->instructions) {
			if (dynamic_cast<Instruction_sop_lsh*> (instr) || dynamic_cast<Instruction_sop_rsh*> (instr)) {
				for (auto reg : this->gprs) {
					if (reg->name.compare("rcx") != 0) {
						if (printI) std::cout << "rcx shift" << std::endl;
						this->add_edge(f->reduce_v[instr->gen[0]], f->reduce_v[reg]);
					}
				}
			}
		}
	}
	
	void InterferenceGraph::add_edge(Variable* start, Variable* end) {
		this->adj_matrix[this->indices[start]*this->indices.size() + this->indices[end]] = true;
		this->adj_matrix[this->indices[end]*this->indices.size() + this->indices[start]] = true;
		if (printI) std::cout << "num indices: " << this->indices.size() << std::endl;
		if (printI) std::cout << "num items: " << this->funct->items.size() << std::endl;
	}

	void generate_interference_output(InterferenceGraph* ig) {
		//if (printI) std::cout << "num indices: " << ig->indices.size() << std::endl;
		//if (printI) std::cout << "num items: " << ig->items.size() << std::endl;
		for (int i=0; i<ig->indices.size(); i++) {
			//if (printI) std::cout << "printing name: ";
			//if (printI) std::cout << ig->items[i]->name << std::endl;
			std::cout << ig->funct->items[i]->name;
			for (int j=0; j<ig->indices.size(); j++) {
				//if (printI) std::cout << "potential interf: " << ig->items[j]->name << std::endl;
				if (i!=j && ig->adj_matrix[ig->indices.size()*i + j])
					//if (printI) std::cout << "yes! " << ig->items[j]->name << std::endl;
					std::cout << " " << ig->funct->items[j]->name;
			}
			std::cout << "\n";
		}
	}

}