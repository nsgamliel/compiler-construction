#include <iostream>
#include <string>

#include <liveness.h>
#include <L2.h>

namespace L2 {

	bool verbose = false;

	Function_l generate_liveness(L2::Program p) {
		Function_l f_l;

		f_l = gen_kill(*(p.functions[0]));
		f_l = find_successors(f_l, *(p.functions[0]));
		f_l = in_out(f_l);

		return f_l;
	}

	Function_l gen_kill(L2::Function& f) {
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
					if (instr->items[1]->register_name.compare("rsp") != 0)
						instr_l->gen.push_back(add_items(&f_l, instr->items[1])[0]);
					instr_l->kill = add_items(&f_l, instr->items[1]);
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
					if (instr->items[1]->register_name.compare("rsp") != 0)
						instr_l->gen.push_back(add_items(&f_l, instr->items[1])[0]);
					instr_l->kill = add_items(&f_l, instr->items[2]);
					break;
				case cond_less_jmp: // both srcs read, no write
				case cond_le_jmp:
				case cond_eq_jmp:
				case store:
					instr_l->gen = add_items(&f_l, instr->items[0]);
					if (instr->items[1]->register_name.compare("rsp") != 0)
						instr_l->gen.push_back(add_items(&f_l, instr->items[1])[0]);
					break;
				case at: // base and offset read, dst written
					instr_l->gen = add_items(&f_l, instr->items[1]);
					if (instr->items[2]->register_name.compare("rsp") != 0)
						instr_l->gen.push_back(add_items(&f_l, instr->items[2])[0]);
					instr_l->kill = add_items(&f_l, instr->items[3]);
					break;
				case ret: // special case
					L2::callee_save.push_back("rax");
					instr_l->gen = add_items(&f_l, L2::callee_save);
					L2::callee_save.pop_back();
					break;
				case call_local: { // special case
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::max(std::stoi(instr->items[1]->value), 6); i++) {
						instr_items.push_back(L2::caller_save[i]);
					}
					instr_l->gen = add_items(&f_l, instr_items);
					if (instr->items[0]->register_name.compare("rsp") != 0)
						instr_l->gen.push_back(add_items(&f_l, instr->items[0])[0]);
					instr_l->kill = add_items(&f_l, L2::caller_save);
					break; }
				case call_runtime: { // special case
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::max(std::stoi(instr->items[1]->value), 6); i++) {
						instr_items.push_back(L2::caller_save[i]);
					}
					instr_l->gen = add_items(&f_l, instr_items);
					instr_l->kill = add_items(&f_l, L2::caller_save);
					// gen gets only the args dictated by N
					// kill gets all caller save
					break; }
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
		return f_l;
	}

	Function_l in_out(Function_l f_l) {
		//TODO
		return f_l;
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

	std::vector<size_t> add_items(Function_l* f_l, L2::Item* item) {
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