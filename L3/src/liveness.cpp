#include <algorithm>
#include <iostream>
#include <string>
#include <map>

#include <liveness.h>

namespace L3 {

	bool printL = false;
	bool printLTOP = false;
	// if (printL) std::cout << "" << std::endl;

	void L3::Function::generateLiveness() {
		if (printLTOP) std::cout << "in liveness" << std::endl;
		if (printLTOP) std::cout << "clearing instruction vectors" << std::endl;
		for (auto i : this->instructions) {
			i->gen.clear();
			i->kill.clear();
			i->in.clear();
			i->out.clear();
		}
		if (printLTOP) std::cout << "entering genKill" << std::endl;
		this->_genKill();
		if (printLTOP) std::cout << "entering findSuccessors" << std::endl;
		this->_findSuccessors();
		if (printLTOP) std::cout << "entering inOut" << std::endl;
		this->_inOut();
		if (printLTOP) std::cout << "leaving liveness" << std::endl;

		if (printLTOP) {
			for (auto i : this->instructions) {
				std::cout << "instruction: " << i << "\nin: " << i->in.size() << "\nout: " << i->out.size() << std::endl;
			}
		}

		return;
	}

	void L3::Function::_genKill() {
		auto gkg = new GenKillGenerator();
		if (printL) std::cout << "employing genkillgenerator" << std::endl;
		for (auto i : this->instructions) {
			i->accept(gkg);
		}

		// replace gen and kill vars with unique var pointers (in case dynamic casts generate a new pointer)
		if (printL) std::cout << "maintaining uniqueness of var pointers" << std::endl;
		for (auto i : this->instructions) {
			if (printL) std::cout << "at instruction gen sets" << std::endl;
			for (int v=0; v<i->gen.size(); v++) {
				if (printL) std::cout << "at index: " << v << std::endl;
				if (printL) std::cout << "size: " << i->gen.size() << std::endl;
				if (i->gen[v]->getDup(this->vars)) {
					if (printL) std::cout << "replacing pointer " << i->gen[v] << " with " << i->gen[v]->getDup(this->vars) << std::endl;
					i->gen[v] = i->gen[v]->getDup(this->vars);
				}
				if (printL) std::cout << "leaving index: " << v << std::endl;
			}
			if (printL) std::cout << "at instruction kill sets" << std::endl;
			for (int v=0; v<i->kill.size(); v++) {
				if (i->kill[v]->getDup(this->vars)) {
					i->kill[v] = i->kill[v]->getDup(this->vars);
				}
			}
		}

		return;
	}

	void L3::Function::_findSuccessors() {
		std::map<int, std::string> jumps;
		std::map<int, std::string> labels;
		for (int x=0; x<this->instructions.size(); x++) {
			this->instructions[x]->successors.clear();
			// case no successors
			if (dynamic_cast<Instruction_return*> (this->instructions[x]) ||dynamic_cast<Instruction_return_val*> (this->instructions[x]) ||  dynamic_cast<Instruction_call_tensor_error*> (this->instructions[x]))
				continue;
			// case one successor not next
			auto inst_jmp_c = dynamic_cast<Instruction_branch*> (this->instructions[x]);
			if (inst_jmp_c) {
				// check if label already found
				for (auto entry : labels) {
					if (inst_jmp_c->label->name.compare(entry.second) == 0)
						this->instructions[x]->successors.push_back(this->instructions[entry.first]);
				}
				// store in case label is found later
				jumps[x] = inst_jmp_c->label->name;
				continue;
			}
			//case two successors
			auto inst_cnd_c = dynamic_cast<Instruction_cond_branch*> (this->instructions[x]);
			if (inst_cnd_c) {
				for (auto entry : labels) {
					if (inst_cnd_c->label->name.compare(entry.second) == 0)
						this->instructions[x]->successors.push_back(this->instructions[entry.first]);
				}
				jumps[x] = inst_cnd_c->label->name;
			}
			// case label
			auto inst_lbl_c = dynamic_cast<Instruction_label*> (this->instructions[x]);
			if (inst_lbl_c) {
				for (auto entry : jumps) {
					// check if previous instructions branch to this
					if (inst_lbl_c->label->name.compare(entry.second) == 0)
						this->instructions[entry.first]->successors.push_back(this->instructions[x]);
				}
				// store label in case later instructions branch here
				labels[x] = inst_lbl_c->label->name;
			}

			this->instructions[x]->successors.push_back(this->instructions[x+1]);
		}
	}

	void L3::Function::_inOut() {
		do {
			this->isDirty = false;
			std::vector<Variable*> new_vec;
			if (printL) std::cout << "hello?\n";
			for (int i=this->instructions.size()-1; i>=0; i--) {
				if (printL) std::cout << "in instruction " << this->instructions[i] << "\n";
				new_vec = {};
				// OUT[i] = U(IN[s]) where s is all successors of i
				for (auto succ : this->instructions[i]->successors) {
					if (printL) std::cout << "in instruction successors\n";
					for (auto var : succ->in) {
						if (printL) std::cout << "in successor ins\n";
						if (!(var->getDup(new_vec))) {
							if (printL) std::cout << "new var added\n";
							new_vec.push_back(var);
						}
					}
				}
				// check for differences
				if (printL) std::cout << "checking for differences\n";
				if (new_vec.size() == this->instructions[i]->out.size()) {
					for (auto var : new_vec) {
						if (!(var->getDup(this->instructions[i]->out))) {
							if (printL) std::cout << "new outset\n";
							this->isDirty = true;
							this->instructions[i]->out = new_vec;
							break;
						}
					}
				} else {
					if (printL) std::cout << "different outset length\n";
					this->isDirty = true;
					this->instructions[i]->out = new_vec;
				}

				// IN[i] = GEN[i] U (OUT[i] - KILL[i])
				new_vec = {};
				new_vec = this->instructions[i]->gen;
				if (printL) std::cout << "examinign inset\n";
				for (auto var : this->instructions[i]->out) {
					if (!(var->getDup(this->instructions[i]->kill)) && !(var->getDup(new_vec))) {
						if (printL) std::cout << "adding from out\n";
						new_vec.push_back(var);
					}
				}
				// check for differences
				if (printL) std::cout << "checking differences\n";
				if (new_vec.size() == this->instructions[i]->in.size()) {
					for (auto var : new_vec) {
						if (!(var->getDup(this->instructions[i]->in))) {
							if (printL) std::cout << "replacing inset\n";
							this->isDirty = true;
							this->instructions[i]->in = new_vec;
							break;
						}
					}
				} else {
					if (printL) std::cout << this->instructions[i]->in.size() << " length different replacing inset\n";
					this->isDirty = true;
					this->instructions[i]->in = new_vec;
				}
			}
		} while (this->isDirty);
	}

	void GenKillGenerator::visit(Instruction_return* i) { return; }

	void GenKillGenerator::visit(Instruction_return_val* i) {
		if (dynamic_cast<Variable*>(i->retValue)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->retValue));
		} return;
	}

	void GenKillGenerator::visit(Instruction_mov* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->src)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->src));
		} return;
	}

	void GenKillGenerator::visit(Instruction_plus* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_minus* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_times* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_and* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_lsh* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_rsh* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_eq* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_le* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_ge* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_less* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_greater* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->left)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->left));
		}
		if (dynamic_cast<Variable*>(i->right)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->right));
		} return;
	}

	void GenKillGenerator::visit(Instruction_load* i) {
		i->kill.push_back(i->dst);
		i->gen.push_back(i->src);
		return;
	}

	void GenKillGenerator::visit(Instruction_store* i) {
		i->kill.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->src)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->src));
		} return;
	}
	
	void GenKillGenerator::visit(Instruction_label* i) { return; }

	void GenKillGenerator::visit(Instruction_branch* i)  { return; }

	void GenKillGenerator::visit(Instruction_cond_branch* i) {
		i->gen.push_back(i->cond);
		return;
	}

	void GenKillGenerator::visit(Instruction_call* i) {
		if (dynamic_cast<Variable*>(i->dst)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->dst));
		}
		for (auto a : i->args) {
			if (dynamic_cast<Variable*>(a)) {
				i->gen.push_back(dynamic_cast<Variable*>(a));
			}
		}
		return;
	}

	void GenKillGenerator::visit(Instruction_call_print* i) {
		if (dynamic_cast<Variable*>(i->arg)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->arg));
		}
		return;
	}

	void GenKillGenerator::visit(Instruction_call_allocate* i) {
		if (dynamic_cast<Variable*>(i->num)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->num));
		}
		if (dynamic_cast<Variable*>(i->val)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->val));
		}
		return;
	}

	void GenKillGenerator::visit(Instruction_call_input* i) { return; }

	void GenKillGenerator::visit(Instruction_call_tensor_error* i) {return; }

	void GenKillGenerator::visit(Instruction_call_assign* i) {
		i->gen.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->src)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->src));
		}
		for (auto a : i->args) {
			if (dynamic_cast<Variable*>(a)) {
				i->gen.push_back(dynamic_cast<Variable*>(a));
			}
		}
		return;
	}

	void GenKillGenerator::visit(Instruction_call_allocate_assign* i) {
		i->gen.push_back(i->dst);
		if (dynamic_cast<Variable*>(i->num)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->num));
		}
		if (dynamic_cast<Variable*>(i->val)) {
			i->gen.push_back(dynamic_cast<Variable*>(i->val));
		}
		return;
	}

	void GenKillGenerator::visit(Instruction_call_input_assign* i) {
		i->gen.push_back(i->dst);
		return;
	}

}