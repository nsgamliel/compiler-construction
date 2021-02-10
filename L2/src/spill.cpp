#include <iostream>
#include <string>
#include <vector>

#include <spill.h>

namespace L2 {

	bool printS = false;
	// if (printS) std::cout << "" << std::endl;

	void generate_spill_output(Function* f) {
		std::cout << "(" << f->name << "\n\t" << std::to_string(f->arguments) << " " << std::to_string(f->locals) << "\n";
		for (auto i : f->instructions)
			std::cout << "\t" << i->toString() << std::endl;
		std::cout << ")\n";
	}

	Spiller::Spiller(Variable* svar, std::string prefix) {
		spill_var = svar;
		spill_prefix = prefix;
	}

	void L2::Function::spill(Variable* svar, std::string prefix) {
		auto sp = new Spiller(svar, prefix);
		std::vector<Instruction*> instrs;

		if (printS) std::cout << "num items: " << this->items.size() << std::endl;
		if (!(svar->is_in(this->items))) {
			if (printS) std::cout << "var isn't here, returning" << std::endl;
			return;
		}
		this->reduce_v[svar] = svar->get_dup(this->items);

		for (auto instr : this->instructions) {
			if (printS) std::cout << "in new instr" << std::endl;
			// check for load
			bool replaced = false;
			bool load_added = false;
			for (auto var : instr->gen) {
				if (!load_added && this->reduce_v[var] == this->reduce_v[svar]) {
					if (printS) std::cout << "adding new load" << std::endl;
					auto load = new Instruction_load(new Memory(new Variable("rsp"), new Number(0)), new Variable(prefix + std::to_string(sp->num_replace)));
					instrs.push_back(load);
					load_added = true;
					replaced = true;
				}
			}
			// actual instruction
			if (printS) std::cout << "adding root instruction" << std::endl;
			instr->accept(sp);
			instrs.push_back(instr);
			// check for store
			bool kill_added = false;
			for (auto var : instr->kill) {
				if (!kill_added && this->reduce_v[var] == this->reduce_v[svar]) {
					if (printS) std::cout << "adding store" << std::endl;
					auto store = new Instruction_store(new Variable(prefix + std::to_string(sp->num_replace)), new Memory(new Variable("rsp"), new Number(0)));
					instrs.push_back(store);
					kill_added = true;
					replaced = true;
				}
			}
			if (replaced) sp->num_replace++;
		}
		if (sp->num_replace) {
			this->locals++;
			this->instructions = instrs;
		}
		return;
	}

	void Spiller::visit(Instruction_return* i) { return; }
	void Spiller::visit(Instruction_mov* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_label* i) { return; }
	void Spiller::visit(Instruction_aop_pe* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));

		auto src2_c = dynamic_cast<Memory*> (i->src);
		auto dst2_c = dynamic_cast<Memory*> (i->dst);
		if (src2_c && src2_c->var->name.compare(this->spill_var->name) == 0)
			i->src = new Memory(new Variable(this->spill_prefix + std::to_string(this->num_replace)), src2_c->offset);
		if (dst2_c && dst2_c->var->name.compare(this->spill_var->name) == 0)
			i->dst = new Memory(new Variable(this->spill_prefix + std::to_string(this->num_replace)), dst2_c->offset);
	}
	void Spiller::visit(Instruction_aop_me* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_aop_te* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_aop_ae* i)  {
		auto src_c = dynamic_cast<Variable*> (i->src);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_aop_pp* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_aop_mm* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_sop_lsh* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_sop_rsh* i) {
		auto src_c = dynamic_cast<Variable*> (i->src);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_c && src_c->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_dir_jmp* i) { return; }
	void Spiller::visit(Instruction_cmp_less* i) {
		auto src_l_c = dynamic_cast<Variable*> (i->left);
		auto src_r_c = dynamic_cast<Variable*> (i->right);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_l_c && src_l_c->name.compare(this->spill_var->name) == 0)
			i->left = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (src_r_c && src_r_c->name.compare(this->spill_var->name) == 0)
			i->right = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_cmp_le* i) {
		auto src_l_c = dynamic_cast<Variable*> (i->left);
		auto src_r_c = dynamic_cast<Variable*> (i->right);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_l_c && src_l_c->name.compare(this->spill_var->name) == 0)
			i->left = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (src_r_c && src_r_c->name.compare(this->spill_var->name) == 0)
			i->right = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_cmp_eq* i) {
		auto src_l_c = dynamic_cast<Variable*> (i->left);
		auto src_r_c = dynamic_cast<Variable*> (i->right);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_l_c && src_l_c->name.compare(this->spill_var->name) == 0)
			i->left = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (src_r_c && src_r_c->name.compare(this->spill_var->name) == 0)
			i->right = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_cnd_jmp_less* i) {
		if (printS) std::cout << "--------cmpless-------" << std::endl;
		auto src_l_c = dynamic_cast<Variable*> (i->left);
		auto src_r_c = dynamic_cast<Variable*> (i->right);
		if (src_l_c && src_l_c->name.compare(this->spill_var->name) == 0)
			i->left = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (src_r_c && src_r_c->name.compare(this->spill_var->name) == 0)
			i->right = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_cnd_jmp_le* i) {
		auto src_l_c = dynamic_cast<Variable*> (i->left);
		auto src_r_c = dynamic_cast<Variable*> (i->right);
		if (src_l_c && src_l_c->name.compare(this->spill_var->name) == 0)
			i->left = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (src_r_c && src_r_c->name.compare(this->spill_var->name) == 0)
			i->right = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_cnd_jmp_eq* i) {
		auto src_l_c = dynamic_cast<Variable*> (i->left);
		auto src_r_c = dynamic_cast<Variable*> (i->right);
		if (src_l_c && src_l_c->name.compare(this->spill_var->name) == 0)
			i->left = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (src_r_c && src_r_c->name.compare(this->spill_var->name) == 0)
			i->right = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_at* i) {
		auto src_l_c = dynamic_cast<Variable*> (i->base);
		auto src_r_c = dynamic_cast<Variable*> (i->index);
		auto dst_c = dynamic_cast<Variable*> (i->dst);
		if (src_l_c && src_l_c->name.compare(this->spill_var->name) == 0)
			i->base = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (src_r_c && src_r_c->name.compare(this->spill_var->name) == 0)
			i->index = new Variable(this->spill_prefix + std::to_string(this->num_replace));
		if (dst_c && dst_c->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_load* i) {
		auto dst = dynamic_cast<Variable*> (i->dst);
		if (dynamic_cast<Memory*>(i->src)->var->name.compare(this->spill_var->name) == 0)
			i->src = new Memory(new Variable(this->spill_prefix + std::to_string(this->num_replace)), dynamic_cast<Memory*>(i->src)->offset);
		if (dst && dst->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_store* i) {
		auto src = dynamic_cast<Variable*> (i->dst);
		if (dynamic_cast<Memory*>(i->dst)->var->name.compare(this->spill_var->name) == 0)
			i->dst = new Memory(new Variable(this->spill_prefix + std::to_string(this->num_replace)), dynamic_cast<Memory*>(i->dst)->offset);
		if (src && src->name.compare(this->spill_var->name) == 0)
			i->src = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_call* i) {
		auto dst = dynamic_cast<Variable*> (i->dst);
		if (dst && dst->name.compare(this->spill_var->name) == 0)
			i->dst = new Variable(this->spill_prefix + std::to_string(this->num_replace));
	}
	void Spiller::visit(Instruction_call_print* i) { return; }
	void Spiller::visit(Instruction_call_input* i) { return; }
	void Spiller::visit(Instruction_call_allocate* i) { return; }
	void Spiller::visit(Instruction_call_tensor_error* i) { return; }
	void Spiller::visit(Instruction_load_stack_arg* i) { return; }

}