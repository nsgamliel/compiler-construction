#include <algorithm>
#include <iostream>
#include <string>
#include <map>

#include <liveness.h>

namespace L2 {

	bool printV = false;

	void L2::Function::generate_liveness() {
		// todo
		if (printV) std::cout << "entering gen_kill" << std::endl;
		this->gen_kill();
		if (printV) std::cout << "entering find_successors" << std::endl;
		this->find_successors();
		if (printV) std::cout << "entering in_out" << std::endl;
		this->in_out();
		if (printV) std::cout << "leaving generate_liveness" << std::endl;
		return;
	}

	void L2::Function::gen_kill() {
		auto gkg = new GenKill_Generator();
		for (auto i : this->instructions) {
			i->gen.clear();
			i->kill.clear();
			i->accept(gkg);
		}

		if (printV) {
			for (auto i : this->instructions) {
				std::cout << "gen: " << i->gen.size() << " elements\nkill: " << i->kill.size() << " elements\n\n";
			}
		}

	}

	void L2::Function::find_successors() {
		std::map<int, std::string> jumps;
		std::map<int, std::string> labels;
		for (int x=0; x<this->instructions.size(); x++) {
			this->instructions[x]->successors.clear();
			// case no successors
			if (dynamic_cast<Instruction_return*> (this->instructions[x]) || dynamic_cast<Instruction_call_tensor_error*> (this->instructions[x]))
				continue;
			// case one successor not next
			auto inst_jmp_c = dynamic_cast<Instruction_dir_jmp*> (this->instructions[x]);
			if (inst_jmp_c) {
				// check if label already found
				for (auto entry : labels) {
					if (inst_jmp_c->label->name.compare(entry.second) == 0)
						this->instructions[x]->successors.push_back(this->instructions[entry.first]);
				}
				jumps[x] = inst_jmp_c->label->name;
				continue;
			}
			//case two successors
			auto inst_cnd_c = dynamic_cast<Instruction_cnd_jmp*> (this->instructions[x]);
			if (inst_cnd_c) {
				for (auto entry : labels) {
					if (inst_cnd_c->dst->name.compare(entry.second) == 0)
						this->instructions[x]->successors.push_back(this->instructions[entry.first]);
				}
				jumps[x] = inst_cnd_c->dst->name;
			}
			// case label
			auto inst_lbl_c = dynamic_cast<Instruction_label*> (this->instructions[x]);
			if (inst_lbl_c) {
				for (auto entry : jumps) {
					if (inst_lbl_c->label->name.compare(entry.second) == 0)
						this->instructions[entry.first]->successors.push_back(this->instructions[x]);
				}
				labels[x] = inst_lbl_c->label->name;
			}

			this->instructions[x]->successors.push_back(this->instructions[x+1]);
		}

		if (printV) {
			for (auto i : this->instructions) {
				std::cout << "succ: " << i->successors.size() << " elements\n\n";
			}
		}
	}

	void L2::Function::in_out() {
		if (printV) std::cout << "in inout\n";
		for (auto i : this->instructions) {
			i->in.clear();
			i->out.clear();
		}
		do {
			this->isDirty = false;
			std::vector<Variable*> new_vec;
			if (printV) std::cout << "hello?\n";
			for (int i=this->instructions.size()-1; i>=0; i--) {
				if (printV) std::cout << "in instruction " << this->instructions[i] << "\n";
				new_vec = {};
				// OUT[i] = U(IN[s]) where s is all successors of i
				for (auto succ : this->instructions[i]->successors) {
					if (printV) std::cout << "in instruction successors\n";
					for (auto var : succ->in) {
						if (printV) std::cout << "in successor ins\n";
						if (!(var->is_in(new_vec))) {
							if (printV) std::cout << "new var added\n";
							new_vec.push_back(var);
						}
					}
				}
				// check for differences
				if (printV) std::cout << "checking for differences\n";
				if (new_vec.size() == this->instructions[i]->out.size()) {
					for (auto var : new_vec) {
						if (!(var->is_in(this->instructions[i]->out))) {
							if (printV) std::cout << "new outset\n";
							this->isDirty = true;
							this->instructions[i]->out = new_vec;
							break;
						}
					}
				} else {
					if (printV) std::cout << "different outset length\n";
					this->isDirty = true;
					this->instructions[i]->out = new_vec;
				}

				// IN[i] = GEN[i] U (OUT[i] - KILL[i])
				new_vec = {};
				new_vec = this->instructions[i]->gen;
				if (printV) std::cout << "examinign inset\n";
				for (auto var : this->instructions[i]->out) {
					if (!(var->is_in(this->instructions[i]->kill)) && !(var->is_in(new_vec))) {
						if (printV) std::cout << "adding from out\n";
						new_vec.push_back(var);
					}
				}
				// check for differences
				if (printV) std::cout << "checking differences\n";
				if (new_vec.size() == this->instructions[i]->in.size()) {
					for (auto var : new_vec) {
						if (!(var->is_in(this->instructions[i]->in))) {
							if (printV) std::cout << "replacing inset\n";
							this->isDirty = true;
							this->instructions[i]->in = new_vec;
							break;
						}
					}
				} else {
					if (printV) std::cout << this->instructions[i]->in.size() << " length different replacing inset\n";
					this->isDirty = true;
					this->instructions[i]->in = new_vec;
				}
			}
		} while (this->isDirty);


		if (printV) {
			for (auto i : this->instructions) {
				std::cout << "in: " << i->in.size() << " elements\nout: " << i->out.size() << " elements\n\n";
			}
		}
	}

	void generate_liveness_output(L2::Function* f) {
		std::cout << "(\n(in\n";
		int i;
		for (auto instr : f->instructions) {
			std::cout << "(";
			for (i=0; i<instr->in.size(); i++) {
				std::cout << instr->in[i]->name;
				if (i != instr->in.size()-1)
					std::cout << " ";
			}
			std::cout << ")\n";
		}
		std::cout << ")\n\n(out\n";
		for (auto instr : f->instructions) {
			std::cout << "(";
			for (i=0; i<instr->out.size(); i++) {
				std::cout << instr->out[i]->name;
				if (i != instr->out.size()-1)
					std::cout << " ";
			}
			std::cout << ")\n";
		}
		std::cout << ")\n\n)\n";
		return;
	}

	void GenKill_Generator::visit(Instruction_return* i) { 
		this->callee_save.push_back(new Variable("rax"));
		for (auto elem : this->callee_save)
			i->gen.push_back(elem);
		this->callee_save.pop_back();
		return;
	}
	
	void GenKill_Generator::visit(Instruction_mov* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) i->kill.push_back(d);
		return;
	}

	void GenKill_Generator::visit(Instruction_label* i) { return; }

	void GenKill_Generator::visit(Instruction_aop_pe* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) {
			i->gen.push_back(d);
			i->kill.push_back(d);
		}
		return;
	}

	void GenKill_Generator::visit(Instruction_aop_me* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) {
			i->gen.push_back(d);
			i->kill.push_back(d);
		}
		return;
	}

	void GenKill_Generator::visit(Instruction_aop_te* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) {
			i->gen.push_back(d);
			i->kill.push_back(d);
		}
		return;
	}
	
	void GenKill_Generator::visit(Instruction_aop_ae* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) {
			i->gen.push_back(d);
			i->kill.push_back(d);
		}
		return;
	}
	
	void GenKill_Generator::visit(Instruction_aop_pp* i) {
		auto s = conv_item(i->src);
		if (s) {
			i->gen.push_back(s);
			i->kill.push_back(s);
		}
		return;
	}

	void GenKill_Generator::visit(Instruction_aop_mm* i) {
		auto s = conv_item(i->src);
		if (s) {
			i->gen.push_back(s);
			i->kill.push_back(s);
		}
		return;
	}
	
	void GenKill_Generator::visit(Instruction_sop_lsh* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) {
			i->gen.push_back(d);
			i->kill.push_back(d);
		}
		return;
	}
	
	void GenKill_Generator::visit(Instruction_sop_rsh* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) {
			i->gen.push_back(d);
			i->kill.push_back(d);
		}
		return;
	}
	
	void GenKill_Generator::visit(Instruction_dir_jmp* i) { return; }

	void GenKill_Generator::visit(Instruction_cmp_less* i) {
		auto l = conv_item(i->left);
		auto r = conv_item(i->right);
		auto d = conv_item(i->dst);
		if (l) i->gen.push_back(l);
		if (r) i->gen.push_back(r);
		if (d) i->kill.push_back(d);
		return;
	}

	void GenKill_Generator::visit(Instruction_cmp_le* i) {
		auto l = conv_item(i->left);
		auto r = conv_item(i->right);
		auto d = conv_item(i->dst);
		if (l) i->gen.push_back(l);
		if (r) i->gen.push_back(r);
		if (d) i->kill.push_back(d);
		return;
	}
	
	void GenKill_Generator::visit(Instruction_cmp_eq* i) {
		auto l = conv_item(i->left);
		auto r = conv_item(i->right);
		auto d = conv_item(i->dst);
		if (l) i->gen.push_back(l);
		if (r) i->gen.push_back(r);
		if (d) i->kill.push_back(d);
		return;
	}
	
	void GenKill_Generator::visit(Instruction_cnd_jmp_less* i) {
		auto l = conv_item(i->left);
		auto r = conv_item(i->right);
		if (l) i->gen.push_back(l);
		if (r) i->gen.push_back(r);
		return;
	}
	
	void GenKill_Generator::visit(Instruction_cnd_jmp_le* i) {
		auto l = conv_item(i->left);
		auto r = conv_item(i->right);
		if (l) i->gen.push_back(l);
		if (r) i->gen.push_back(r);
		return;
	}
	
	void GenKill_Generator::visit(Instruction_cnd_jmp_eq* i) {
		auto l = conv_item(i->left);
		auto r = conv_item(i->right);
		if (l) i->gen.push_back(l);
		if (r) i->gen.push_back(r);
		return;
	}
	
	void GenKill_Generator::visit(Instruction_at* i) {
		auto b   = conv_item(i->base);
		auto ind = conv_item(i->index);
		auto d   = conv_item(i->dst);
		if (b)   i->gen.push_back(b);
		if (ind) i->gen.push_back(ind);
		if (d)   i->kill.push_back(d);
		return;
	}
	
	void GenKill_Generator::visit(Instruction_load* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) i->kill.push_back(d);
		return;
	}

	void GenKill_Generator::visit(Instruction_store* i) {
		auto s = conv_item(i->src);
		auto d = conv_item(i->dst);
		if (s) i->gen.push_back(s);
		if (d) i->gen.push_back(d);
		return;
	}

	void GenKill_Generator::visit(Instruction_call* i) {
		for (int x=0; x<std::min(i->args->value, int64_t(6)); x++) {
			i->gen.push_back(this->caller_save[x]);
		}
		auto d = conv_item(i->dst);
		if (d) i->gen.push_back(d);
		for (auto elem : this->caller_save)
			i->kill.push_back(elem);
		return;
	}

	void GenKill_Generator::visit(Instruction_call_print* i) {
		for (int x=0; x<std::min(i->args->value, int64_t(6)); x++) {
			i->gen.push_back(this->caller_save[x]);
		}
		for (auto elem : this->caller_save)
			i->kill.push_back(elem);
		return;
	}

	void GenKill_Generator::visit(Instruction_call_input* i) {
		for (int x=0; x<std::min(i->args->value, int64_t(6)); x++) {
			i->gen.push_back(this->caller_save[x]);
		}
		for (auto elem : this->caller_save)
			i->kill.push_back(elem);
		return;
	}

	void GenKill_Generator::visit(Instruction_call_allocate* i) {
		for (int x=0; x<std::min(i->args->value, int64_t(6)); x++) {
			i->gen.push_back(this->caller_save[x]);
		}
		for (auto elem : this->caller_save)
			i->kill.push_back(elem);
		return;
	}

	void GenKill_Generator::visit(Instruction_call_tensor_error* i) {
		for (int x=0; x<std::min(i->args->value, int64_t(6)); x++) {
			i->gen.push_back(this->caller_save[x]);
		}
		for (auto elem : this->caller_save)
			i->kill.push_back(elem);
		return;
	}

	void GenKill_Generator::visit(Instruction_load_stack_arg* i) {
		auto d = conv_item(i->dst);
		if (d) i->kill.push_back(d);
		return;
	}

	L2::Variable* GenKill_Generator::conv_item(Item* item) {
		auto item_c = dynamic_cast<Variable*> (item);
		if (item_c && item_c->name.compare("rsp") != 0) return item_c;
		auto item2_c = dynamic_cast<Memory*> (item);
		if (item2_c && item2_c->var->name.compare("rsp") != 0) {
			return item2_c->var;
		}
		return nullptr;
	}

}