#include <iostream>
#include <string>

#include <spiller.h>

namespace L2 {

	bool printS = true;
	// if (printS) std::cout << "" << std::endl;

	L2::Function spill(L2::Function* f, std::string var, std::string prefix) {
		if (printS) std::cout << "in spill" << std::endl;
		L2::Function f_s;
		f_s.name = f->name;
		f_s.arguments = f->arguments;
		f_s.locals = f->locals;
		bool reg_match;
		bool val_match;

		if (printS) std::cout << "entering instructions" << std::endl;
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
					if (printS) std::cout << "found possible" << std::endl;
					for (auto item : instr->items) {
						if (item->register_name.compare(var) == 0)
							reg_match = true;
						if (item->value.compare(var) == 0)
							val_match = true;
					}
					if (reg_match || val_match) {
						if (printS) std::cout << "found match" << std::endl;
						if (f_s.num_replace > 0) {
							if (printS) std::cout << "loading previous store" << std::endl;
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
					if (printS) std::cout << "no possible" << std::endl;
					f_s.instructions.push_back(instr); break;
			}
			f_s.num_replace++;
		}

		f_s.locals++;
		if (printS) std::cout << "leaving spill" << std::endl;
		return f_s;
	}

	void generate_spill_output(L2::Function f_s) {
		std::cout << "(" << f_s.name << "\n";
		std::cout << "\t" << f_s.arguments << " " << f_s.locals << "\n";
		for (auto instr : f_s.instructions) {
			std::cout << "\t";
			switch (instr->op) {
				case ret:
					std::cout << "return"; break;
				case mov:
				case load:
				case store:
					std::cout << find_name(instr->items[1]) << " <- " << find_name(instr->items[0]); break;
				case label_def:
					std::cout << find_name(instr->items[0]); break;
				case aop_pe:
					std::cout << find_name(instr->items[1]) << " += " << find_name(instr->items[0]); break;
				case aop_me:
				   	std::cout << find_name(instr->items[1]) << " -= " << find_name(instr->items[0]); break;
				case aop_te:
				   	std::cout << find_name(instr->items[1]) << " *= " << find_name(instr->items[0]); break;
				case aop_ae:
				   	std::cout << find_name(instr->items[1]) << " &= " << find_name(instr->items[0]); break;
				case aop_pp:
					std::cout << find_name(instr->items[0]) << "++"; break;
				case aop_mm:
					std::cout << find_name(instr->items[0]) << "++"; break;
				case sop_lsh:
				   	std::cout << find_name(instr->items[1]) << " <<= " << find_name(instr->items[0]); break;
				case sop_rsh:
				   	std::cout << find_name(instr->items[1]) << " >>= " << find_name(instr->items[0]); break;
				case dir_jmp:
					std::cout << "goto " << find_name(instr->items[0]); break;
				case cmp_less:
					std::cout << find_name(instr->items[2]) << " <- " << find_name(instr->items[1]) << " < " << find_name(instr->items[0]); break;
				case cmp_le:
					std::cout << find_name(instr->items[2]) << " <- " << find_name(instr->items[1]) << " <= " << find_name(instr->items[0]); break;
				case cmp_eq:
					std::cout << find_name(instr->items[2]) << " <- " << find_name(instr->items[1]) << " = " << find_name(instr->items[0]); break;
				case cond_less_jmp:
					std::cout << "cjump " << find_name(instr->items[1]) << " < " << find_name(instr->items[0]) << " " << find_name(instr->items[2]); break;
				case cond_le_jmp:
					std::cout << "cjump " << find_name(instr->items[1]) << " <= " << find_name(instr->items[0]) << " " << find_name(instr->items[2]); break;
				case cond_eq_jmp:
					std::cout << "cjump " << find_name(instr->items[1]) << " = " << find_name(instr->items[0]) << " " << find_name(instr->items[2]); break;
				case at:
					std::cout << find_name(instr->items[3]) << " @ " << find_name(instr->items[2]) << " " << find_name(instr->items[1]) << " " << find_name(instr->items[0]); break;
				case call_local:
				case call_runtime:
					std::cout << "call " << find_name(instr->items[0]) << " " << find_name(instr->items[1]); break;
				case load_stack:
					std::cout << find_name(instr->items[1]) << " <- stack-arg " << find_name(instr->items[0]); break;
				default:
					std::cerr << "no match found" << std::endl; break;
			}
			std::cout << "\n";
		}
		std::cout << ")\n";
	}

	std::string find_name(L2::Item* item) {
		std::string str;
		switch (item->type) {
			case 0:
				str = item->register_name; break;
			case 1:
				str = "mem " + item->register_name + " " + item->value; break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case -1:
				str = item->value; break;
		}
		return str;
	}

}