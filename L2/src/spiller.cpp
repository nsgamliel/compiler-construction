#include <algorithm>
#include <iostream>
#include <string>

#include <spiller.h>
#include <liveness.h>

namespace L2 {

	bool printS = false;
	// if (printS) std::cout << "" << std::endl;

	L2::Function spill(L2::Function* f, std::string var, std::string prefix) {
		if (printS) std::cout << "in spill" << std::endl;
		L2::Function f_s;
		L2::Function_l f_l = L2::gen_kill(*(f));
		f_s.name = f->name;
		f_s.arguments = f->arguments;
		f_s.locals = f->locals;
		bool found_match;
		bool var_replaced = false;

		if (printS) std::cout << "entering instructions" << std::endl;
		for (int ind=0; ind<f->instructions.size(); ind++) {
			auto new_instr = new L2::Instruction();
			found_match = false;
			switch (f->instructions[ind]->op) {
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
					for (auto item : f->instructions[ind]->items) {
						if (item->register_name.compare(var) == 0 || item->value.compare(var) == 0)
							found_match = true;
					}
					if (found_match) {
						var_replaced = true;
						if (printS) std::cout << "found match" << std::endl;
						if (f_s.num_replace > -1 && std::find(f_l.instructions[ind]->gen.begin(), f_l.instructions[ind]->gen.end(), f_l.str_hash(var)) != f_l.instructions[ind]->gen.end()) {
							if (printS) std::cout << "loading previous store" << std::endl;
							auto load_instr = new L2::Instruction();
							load_instr->op = load;
							auto dst = new L2::Item();
							auto src = new L2::Item();
							dst->type = 6;
							dst->value = prefix + std::to_string(f_s.num_replace);
							src->type = 1;
							src->value = std::to_string(f_s.locals * 8);
							src->register_name = "rsp";
							load_instr->items.push_back(src);
							load_instr->items.push_back(dst);
							f_s.instructions.push_back(load_instr);
						}
						// actual use followed by store instr
						auto replace_instr = new L2::Instruction();
						replace_instr->op = f->instructions[ind]->op;
						bool must_store = true;
						for (int i=0; i<f->instructions[ind]->items.size(); i++) {
							auto replace_item = new L2::Item();
							replace_item->type = f->instructions[ind]->items[i]->type;
							if (f->instructions[ind]->items[i]->register_name.compare(var) == 0)								
								replace_item->register_name = prefix + std::to_string(f_s.num_replace);
							else {
								//if (i == 2 && (instr->op == cmp_less || instr->op == cmp_le || instr->op == cmp_eq))
									//must_store = false;
								replace_item->register_name = f->instructions[ind]->items[i]->register_name;
							}
							if (f->instructions[ind]->items[i]->value.compare(var) == 0)
								replace_item->value = prefix + std::to_string(f_s.num_replace);
							else {
								//if (i == 2 && (instr->op == cmp_less || instr->op == cmp_le || instr->op == cmp_eq))
									//must_store = false;
								replace_item->value = f->instructions[ind]->items[i]->value;
							}
							replace_instr->items.push_back(replace_item);
						}
						f_s.instructions.push_back(replace_instr);
						if (std::find(f_l.instructions[ind]->kill.begin(), f_l.instructions[ind]->kill.end(), f_l.str_hash(var)) != f_l.instructions[ind]->kill.end()) {
							if (printS) std::cout << "storing var" << std::endl;
							auto store_instr = new L2::Instruction();
							store_instr->op = store;
							auto dst = new L2::Item();
							auto src = new L2::Item();
							src->type = 6;
							src->value = prefix + std::to_string(f_s.num_replace);
							dst->type = 1;
							dst->value = std::to_string(f_s.locals * 8);
							dst->register_name = "rsp";
							store_instr->items.push_back(src);
							store_instr->items.push_back(dst);
							f_s.instructions.push_back(store_instr);	
						}
						f_s.num_replace++;
					} else {
						f_s.instructions.push_back(f->instructions[ind]);
					} break; }
				default:
					if (printS) std::cout << "no possible" << std::endl;
					f_s.instructions.push_back(f->instructions[ind]); break;
			}
		}

		//remove extraneous store
		if (printS) std::cout << "removing extraneous store" << std::endl;
		std::string target = prefix + std::to_string(f_s.num_replace-1);
		if (printS) std::cout << "looking for: " << target << std::endl;
		for (int i=0; i<f_s.instructions.size(); i++) {
			if (printS) std::cout << "examining: " << f_s.instructions[i]->items[0]->value << std::endl;
			if (f_s.instructions[i]->op == store && f_s.instructions[i]->items[0]->value.compare(target) == 0) {
				f_s.instructions.erase(f_s.instructions.begin()+i);
				if (printS) std::cout << "found!" << std::endl;
				break;
			}
		}

		if (var_replaced) f_s.locals++;
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