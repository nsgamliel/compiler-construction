#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

#include <liveness.h>
#include <L2.h>

namespace L2 {

	// if (printV) std::cout << "" << std::endl;

	bool printV = true;

	Function_l generate_liveness(L2::Program p) {
		Function_l f_l;

		if (printV) std::cout << "entering gen_kill" << std::endl;
		f_l = gen_kill(*(p.functions[0]));
		if (printV) std::cout << "entering find_successors" << std::endl;
		f_l = find_successors(f_l, *(p.functions[0]));
		if (printV) std::cout << "entering in_out" << std::endl;
		f_l = in_out(f_l);
		if (printV) std::cout << "leaving generate_liveness" << std::endl;

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
					f_l.callee_save.push_back("rax");
					instr_l->gen = add_from_vec(&f_l, f_l.callee_save);
					f_l.callee_save.pop_back();
					break;
				case call_local: { // special case
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::max(std::stoi(instr->items[1]->value), 6); i++) {
						instr_items.push_back(f_l.caller_save[i]);
					}
					instr_l->gen = add_from_vec(&f_l, instr_items);
					if (instr->items[0]->register_name.compare("rsp") != 0)
						instr_l->gen.push_back(add_items(&f_l, instr->items[0])[0]);
					instr_l->kill = add_from_vec(&f_l, f_l.caller_save);
					break; }
				case call_runtime: { // special case
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::max(std::stoi(instr->items[1]->value), 6); i++) {
						instr_items.push_back(f_l.caller_save[i]);
					}
					instr_l->gen = add_from_vec(&f_l, instr_items);
					instr_l->kill = add_from_vec(&f_l, f_l.caller_save);
					// gen gets only the args dictated by N
					// kill gets all caller save
					break; }
				case load_stack: // dst gets written ONLY
					instr_l->kill = add_items(&f_l, instr->items[1]);
					break;
				default:
					if (printV) std::cout << "instruction not found" << std::endl;
					break;
			} f_l.instructions.push_back(instr_l);
		} return f_l;
	}

	Function_l find_successors(Function_l f_l, Function& f) {
		if (f.instructions.size() != f_l.instructions.size()) {
			std::cerr << "instruction sizes do not match" << std::endl; 
			exit(1);
		}
		int i;
		for (i=0; i<f.instructions.size(); i++) {
			switch (f.instructions[i]->op) {
				case ret:
					f_l.instructions[i]->successors.push_back(-1); break;
				case call_runtime:
					if (f.instructions[i]->items[0]->value.compare("tensor-error") == 0)
						f_l.instructions[i]->successors.push_back(-1); 
					break;
				case dir_jmp: {
					int j;
					for (j=0; j<f.instructions.size(); j++) {
						if (f.instructions[j]->op == label_def && f.instructions[j]->items[0]->value.compare(f.instructions[i]->items[0]->value) == 0) {
							f_l.instructions[i]->successors.push_back(j);
						}
					} break; }
				case cond_less_jmp:
				case cond_le_jmp:
				case cond_eq_jmp: {
					int j;
					for (j=0; j<f.instructions.size(); j++) {
						if (f.instructions[j]->op == label_def && f.instructions[j]->items[0]->value.compare(f.instructions[i]->items[2]->value) == 0) {
							f_l.instructions[i]->successors.push_back(j);
						}
					} 
					f_l.instructions[i]->successors.push_back(i+1); break; }
				default:
					f_l.instructions[i]->successors.push_back(i+1); break;
			}
		}
		return f_l;
	}

	Function_l in_out(Function_l f_l) {
		do {
			int i;
			f_l.isDirty = false;
			for (int i=f_l.instructions.size()-1; i>=0; i--) {
				if (printV) std::cout << "starting out set " << i << std::endl;
				// OUT[i] = U(IN[s]) where s is all successors of i
				std::vector<size_t> new_set;
				if (printV) std::cout << "examining successors" << std::endl;
				for (int succ : f_l.instructions[i]->successors) {
					if (printV) std::cout << "successor " << succ << std::endl;
					if (succ >= 0) {
						for (size_t elem : f_l.instructions[succ]->in)
							new_set.push_back(elem);	
					}				
				}

				if (printV) std::cout << "checking for dirty" << std::endl;
				if (new_set.size() == f_l.instructions[i]->out.size()) { // make sure the for loop won't miss anything
					for (size_t elem : f_l.instructions[i]->out) {
						if (std::find(new_set.begin(), new_set.end(), elem) == new_set.end()) { // vectors are different
							f_l.instructions[i]->out = new_set;
							f_l.isDirty = true;
						}
					}	
				} else {
					f_l.instructions[i]->out = new_set;
					f_l.isDirty = true;
				}
				if (printV) std::cout << "dirty is " << isDirty << std::endl;
				
				if (printV) std::cout << "starting in set " << i << std::endl;
				// IN[i] = GEN[i] U (OUT[i] - KILL[i])
				new_set = {};
				for (size_t elem : f_l.instructions[i]->gen)
					new_set.push_back(elem);
				for (size_t elem : f_l.instructions[i]->out) {
					if (std::find(f_l.instructions[i]->kill.begin(), f_l.instructions[i]->kill.end(), elem) == f_l.instructions[i]->kill.end()) // not in kill
						new_set.push_back(elem);
				}

				if (printV) std::cout << "checking for dirty" << std::endl;
				if (new_set.size() == f_l.instructions[i]->out.size()) { // make sure the for loop won't miss anything
					for (size_t elem : f_l.instructions[i]->out) {
						if (std::find(new_set.begin(), new_set.end(), elem) == new_set.end()) { // vectors are different
							f_l.instructions[i]->out = new_set;
							f_l.isDirty = true;
						}
					}	
				} else {
					f_l.instructions[i]->out = new_set;
					f_l.isDirty = true;
				}
				if (printV) std::cout << "dirty is " << isDirty << std::endl;
			}
			std::cin.get();
		} while (f_l.isDirty);
		return f_l;
	}

	void generate_inout_output(Function_l f_l, char* fileName) {
		std::ofstream outputFile;
		//std::string file(fileName);
		outputFile.open("inout.out");
		outputFile << "lior is dumb\n" << std::endl;
		outputFile.close();
		return;
	}

	std::vector<size_t> add_from_vec(Function_l* f_l, std::vector<std::string>& strs) {
		std::vector<size_t> new_set;
		for (auto str : strs) {
			if (str.compare("rsp") != 0) {
				new_set.push_back(f_l->str_hash(str));
				f_l->items_l[f_l->str_hash(str)] = str;
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
			new_set.push_back(f_l->str_hash(str));
			f_l->items_l[f_l->str_hash(str)] = str;
		}
		return new_set;
	}

}