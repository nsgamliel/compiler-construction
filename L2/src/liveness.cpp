#pragma once

#include <string>

#include <liveness.h>
#include <L2.h>

namespace L2 {

	bool verbose = false;

	Function_l generate_liveness(Function f) {
		Function_l f_l;

		f_l = gen_kill(f);
		f_l = find_successors(f_l, f);
		f_l = in_out(f_l);

		return f_l;
	}

	Function_l gen_kill(Function& f) {
		Function_l f_l;
		for (auto instr : f.instructions) {
			auto instr_l = new Instruction_l();
			switch (instr->op) {
				case mov: // src is read, dst is written
				case load:
					instr_l->gen = add_items(&f_l, instr->items[0]);
					instr_l->kill = add_items(&f_l, instr->items[1]);
					break;
				case aop_pe: // src is read, dst is read and written
				case aop_me:
				case aop_te:
				case aop_ae:
				case sop_lsh:
				case sop_rsh:
					instr_l->gen = add_items(&f_l, instr->items[0]);
					std::vector<size_t> new_set = add_items(&f_l, instr->items[1]);
					instr_l->gen.insert(instr_l->gen.end(), new_set.begin(), new_set.end());
					instr_l->kill = new_set;
					break;
				case aop_pp: // src is read and written
				case aop_mm:
					instr_l->gen = add_items(&f_l, instr->items[0]);
					instr_l->kill = add_items(&f_l, instr->items[0]);
					break;
				case cmp_less: // both srcs are read, dst is written
				case cmp_le:
				case cmp_eq:
					instr_l->gen = add_items(&f_l, instr->items[0]);
					std::vector<size_t> new_set = add_items(&f_l, instr->items[1]);
					instr_l->gen.insert(instr_l->gen.end(), new_set.begin(), new_set.end());
					instr_l->kill = add_items(&f_l, instr->items[2]);
					break;
				case cond_less_jmp: // both srcs read, no write
				case cond_le_jmp:
				case cond_eq_jmp:
				case store:
					instr_l->gen = add_items(&f_l, instr->items[0]);
					std::vector<size_t> new_set = add_items(&f_l, instr->items[1]);
					instr_l->gen.insert(instr_l->gen.end(), new_set.begin(), new_set.end());
					break;
				case at: // base and offset read, dst written
					instr_l->gen = add_items(&f_l, instr->items[1]);
					std::vector<size_t> new_set = add_items(&f_l, instr->items[2]); // TODO this can be simplified
					instr_l->gen.insert(instr_l->gen.end(), new_set.begin(), new_set.end());
					instr_l->kill = add_items(&f_l, instr->items[3]);
					break;
				case ret: // special case
					std::vector<std::string> instr_items;  // TODO this can be removed, just input callee save directly
					instr_items.push_back("rax");
					for (auto str : L2::callee_save) {
						instr_items.push_back(str);
					}
					instr_l->gen = add_items(&f_l, instr_items);
					break;
				case call_local: // special case
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::max(instr->items[1]->value, 6)) {
						instr_items.push_back(L2::caller_save[i]);
					}
					instr_l->gen = add_items(&f_l, instr_items);
					instr_l->gen.push_back(add_items(&f_l, instr->items[0])[0]);
					instr_l->kill = add_items(&f_l, L2::caller_save);
					break;
				case call_runtime: // special case
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::max(instr->items[1]->value, 6)) {
						instr_items.push_back(L2::caller_save[i]);
					}
					instr_l->gen = add_items(&f_l, instr_items);
					instr_l->kill = add_items(&f_l, L2::caller_save);
					// gen gets only the args dictated by N
					// kill gets all caller save
					break;
				case load_stack: // dst gets written ONLY
					instr_l->kill = add_items(&f_l, instr->items[1]);
					break;
				default:
					if (verbose) std::cout << "instruction not found" << std::endl;
					break;
			} f_l.instructions.push_back(instr_l);
		} return f_l;
	}

	Function_l find_successors(Function_l f_l, Function& f) {
		// goto (1, not immediate successor), return/call tensor-error (0), cjump (2, next, label)
		/*int i;
		for (i=0; i<f.instructions.size(); i++) {
			switch (f.instructions[i]->op) {
				case ret:
					f_l.
			}
		}*/
	}

	Function_l in_out(Function_l f_l) {
		//TODO
	}

	std::vector<size_t> add_items(Function_l* f_l, const std::vector<std::string>& strs) {
		std::vector<size_t> new_set;
		for (auto str : strs) {
			if (str.compare("rsp") != 0) {
				new_set.push_back(str_hash(str));
				f_l->items_l[str_hash(str)] = str;
			}
		}
		return new_set;
	}

	std::vector<size_t> add_items(Function_l* f_l, Item* item) {
		std::vector<size_t> new_set;
		std::string str;
		if (item->type == 0 || item->type == 1)
			str = item->register_name;
		else if (item->type == 6)
			str = item->value;
		if (str.compare("rsp") != 0) {
			new_set.push_back(str_hash(str));
			f_l->items_l[str_hash(str)] = str;
		}
		return new_set;
	}

}