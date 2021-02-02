#include <iostream>
#include <string>

#include <spiller.h>

namespace L2 {

	L2::Function spill(L2::Function* f, std::string var, std::string prefix) {
		L2::Function f_s;
		f_s.locals = f->locals;
		bool reg_match;
		bool val_match;

		for (auto instr : f->instructions) {
			auto new_instr = new L2::Instruction();
			reg_match = false;
			val_match = false;
			switch (instr->op) {
				case mov:
				case aop_pe:
				case aop_me:
				case aop_te:
				case aop_ae:
				case aop_pp:
				case aop_mm:
				case sop_lsh:
				case sop_rsh:
				case cmp_less:
				case cmp_le:
				case cmp_eq:
				case cond_less_jmp:
				case cond_le_jmp:
				case cond_eq_jmp:
				case at:
				case load:
				case store:
				case call_local:
				case load_stack: {
					for (auto item : instr->items) {
						if (item->register_name.compare(var) == 0)
							reg_match = true;
						if (item->value.compare(var) == 0)
							val_match = true;
					}
					if (reg_match || val_match) {
						if (f_s.num_replace > 0) {
							auto load_instr = new L2::Instruction();
							load_instr->op = load;
							auto dst = new L2::Item();
							auto src = new L2::Item();
							dst->type = 6;
							dst->value = prefix + std::to_string(f_s.num_replace);
							src->type = 1;
							src->value = f_s.locals * 8;
							src->register_name = "rsp";
							load_instr->items.push_back(src);
							load_instr->items.push_back(dst);
							f_s.instructions.push_back(load_instr);
						}
						// actual use followed by store instr
						auto replace_instr = new L2::Instruction();
						replace_instr->op = instr->op;
						for (auto item : instr->items) {
							auto replace_item = new L2::Item();
							replace_item->type = item->type;
							if (reg_match) replace_item->register_name = prefix + std::to_string(f_s.num_replace);
							else replace_item->register_name = item->register_name;
							if (val_match) replace_item->value = prefix + std::to_string(f_s.num_replace);
							else replace_item->value = item->value;
							replace_instr->items.push_back(replace_item);
						}
						f_s.instructions.push_back(replace_instr);
						auto store_instr = new L2::Instruction();
						store_instr->op = store;
						auto dst = new L2::Item();
						auto src = new L2::Item();
						src->type = 6;
						src->value = prefix + std::to_string(f_s.num_replace);
						dst->type = 1;
						dst->value = f_s.locals * 8;
						dst->register_name = "rsp";
						store_instr->items.push_back(src);
						store_instr->items.push_back(dst);
						f_s.instructions.push_back(store_instr);
					} else {
						f_s.instructions.push_back(instr);
					} break; }
				default:
					f_s.instructions.push_back(instr); break;
			}
			f_s.num_replace++;
		}

		f_s.locals++;
		return f_s;
	}

	void generate_spill_output(L2::Function f_s) {

	}

}