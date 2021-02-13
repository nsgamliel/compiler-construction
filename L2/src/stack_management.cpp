#include <stack_management.h>

namespace L2 {

	/*int calculate_stack_usage(Function* f) {
		int max_offset = 0;
		for (auto i : f->instructions) {
			// we only really care about store since that is the only instr that signifies local vars
			// must also be relative to rsp
			if (dynamic_cast<Instruction_store*> (i)) {
				int check = dynamic_cast<Instruction_store*>(i)->get_rsp_offset();
				if (check > max_offset) {
					max_offset = check;
				}
			}
		}
		return max_offset;
	}*/

	void save_to_stack(Function* f, std::vector<Variable*> regs) {
		std::vector<Instruction*> i_replace;
		// create the stores
		for (auto var : regs) {
			auto i_new = new Instruction_store(var, new Memory(new Variable("rsp"), new Number(f->locals * 8)));
			i_replace.push_back(i_new);
			f->locals++;
		}
		// copy instructions
		for (auto i : f->instructions) {
			i_replace.push_back(i);
		}
		// create loads
		int index = f->locals - 1;
		for (int i=regs.size()-1; i>=0; i--) {
			auto i_new = new Instruction_load(new Memory(new Variable("rsp"), new Number(index * 8)), regs[i]);
			i_replace.push_back(i_new);
			index--;
		}
		f->instructions = i_replace;
		return;
	}

	void convert_stack_args(Function* f) {
		std::vector<Instruction*> i_replace;
		for (auto i : f->instructions) {
			if (dynamic_cast<Instruction_load_stack_arg*>(i)) {
				auto i_conv = dynamic_cast<Instruction_load_stack_arg*>(i);
				auto i_new = new Instruction_load(new Memory(new Variable("rsp"), new Number((f->locals * 8) + i_conv->offset->value)), i_conv->dst);
				i_replace.push_back(i_new);
			} else {
				i_replace.push_back(i);
			}
		}
		f->instructions = i_replace;
		return;
	}

}