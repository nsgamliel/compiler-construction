#include <algorithm>
#include <iostream>
#include <string>
#include <map>

#include <liveness.h>

namespace L3 {

	bool printL = false;

	void L3::Function::generateLiveness() {
		for (auto i : this->instructions) {
			i->gen.clear();
			i->kill.clear();
			i->in.clear();
			i->out.clear();
		}
		this->_genKill();
		this->_findSuccessors();
		this->_inOut();
		return;
	}

	void L3::Function::_genKill() {
		auto gkg = new GenKillGenerator();
		for (auto i : this->instructions) {
			i->accept(gkg);
		}

		// todo: go through each gen/kill set and replace pointers with unique ptrs contained in vars

		return;
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
		i->gen.push_back(i->src);
		return;
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

	void GenKillGenerator::visit(Instruction_call_print* i)
	void GenKillGenerator::visit(Instruction_call_allocate* i)
	void GenKillGenerator::visit(Instruction_call_input* i)
	void GenKillGenerator::visit(Instruction_call_tensor_error* i)
	void GenKillGenerator::visit(Instruction_call_assign* i)
	void GenKillGenerator::visit(Instruction_call_allocate_assign* i)
	void GenKillGenerator::visit(Instruction_call_input_assign* i)

}