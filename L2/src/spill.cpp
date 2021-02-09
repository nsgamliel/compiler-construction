#include <string>
#include <vector>

#include <spill.h>

namespace L2 {

	// give F a tostring and each I a tostring with a find_name helper for generating output

	void generate_spill_output(Function* f) {
		std::cout << "(" << f->name << "\n\t" << std::to_string(f->arguments) << " " << std::to_string(f->locals) << "\n";
		for (auto i : f->instructions)
			std::cout << i->toString() << std::endl;
		std::cout << ")\n";
	}

	Function* Spiller::spill(Function* f, Variable* svar, std::string prefix) {
		Function* fs;
		this->spill_var = svar;
		this->spill_prefix = prefix;
		if (!(svar->is_in(f->items))) {
			this->fs = f;
			return;
		}
		f->reduce_v[svar] = svar->get_dup(f->items);

		for (auto instr : f->instructions) {
			// check for load
			bool replaced = false;
			for (auto var : instr->gen) {
				if (f->reduce_v[var] == f->reduce_v[svar]) {
					auto load = new Instruction_load(new Memory(new Variable("rsp"), new Number(0)), new Variable(prefix + std::to_string(this->num_replace)));
					fs->instructions.push_back(load);
					replaced = true;
				}
			}
			// actual instruction
			if (replaced) {
				// copy the instruction, inserting the new var
				instr->accept(this);
				fs->instructions.push_back(instr);
			} else fs->instructions.push_back(instr);
			// check for store
			for (auto var : instr->kill) {
				if (f->reduce_v[var] == f->reduce_v[svar]) {
					auto store = new Instruction_store(new Variable(prefix + std::to_string(this->num_replace)), new Memory(new Variable("rsp"), new Number(0)));
					fs->instructions.push_back(store);
					replaced = true;
				}
			}
			if (replaced) this->num_replace++;
		}
		return fs;
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