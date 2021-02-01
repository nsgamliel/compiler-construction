#include <algorithm>
#include <iostream>
#include <string>

#include <liveness.h>
//#include <L2.h>

namespace L2 {

	// if (printV) std::cout << "" << std::endl;

	bool printV = false;

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
			if (printV) std::cout << "examining instruction" << std::endl;
			auto instr_l = new Instruction_l();
			instr_l->op = instr->op;
			switch (instr->op) {
				case mov: // src is read, dst is written
				case load:
					if (printV) std::cout << "move/load" << std::endl;
					instr_l->gen = add_items(&f_l, instr->items[0]);
					instr_l->kill = add_items(&f_l, instr->items[1]);
					break;
				case aop_pe: // src is read, dst is read and written
				case aop_me:
				case aop_te:
				case aop_ae:
				case sop_lsh:
				case sop_rsh: {
					if (printV) std::cout << "aop sop" << std::endl;
					instr_l->gen = add_items(&f_l, instr->items[0]);
					std::vector<size_t> new_item = add_items(&f_l, instr->items[1]);
					if (new_item.size() > 0)
						instr_l->gen.push_back(new_item[0]);
					instr_l->kill = add_items(&f_l, instr->items[1]);
					break; }
				case aop_pp: // src is read and written
				case aop_mm:
					if (printV) std::cout << "mm pp" << std::endl;
					instr_l->gen = add_items(&f_l, instr->items[0]);
					instr_l->kill = add_items(&f_l, instr->items[0]);
					break;
				case cmp_less: // both srcs are read, dst is written
				case cmp_le:
				case cmp_eq: {
					if (printV) std::cout << "cmp" << std::endl;
					instr_l->gen = add_items(&f_l, instr->items[0]);
					std::vector<size_t> new_item = add_items(&f_l, instr->items[1]);
					if (new_item.size() > 0)
						instr_l->gen.push_back(new_item[0]);
					instr_l->kill = add_items(&f_l, instr->items[2]);
					break; }
				case cond_less_jmp: // both srcs read, no write
				case cond_le_jmp:
				case cond_eq_jmp:
				case store: {
					if (printV) std::cout << "condjump store" << std::endl;
					instr_l->gen = add_items(&f_l, instr->items[0]);
					std::vector<size_t> new_item = add_items(&f_l, instr->items[1]);
					if (new_item.size() > 0)
						instr_l->gen.push_back(new_item[0]);
					break; }
				case at: { // base and offset read, dst written
					if (printV) std::cout << "at" << std::endl;
					instr_l->gen = add_items(&f_l, instr->items[1]);
					std::vector<size_t> new_item = add_items(&f_l, instr->items[2]);
					if (new_item.size() > 0)
						instr_l->gen.push_back(new_item[0]);
					instr_l->kill = add_items(&f_l, instr->items[3]);
					break; }
				case ret: // special case
					if (printV) std::cout << "ret" << std::endl;
					f_l.callee_save.push_back("rax");
					instr_l->gen = add_from_vec(&f_l, f_l.callee_save);
					f_l.callee_save.pop_back();
					break;
				case call_local: { // special case
					if (printV) std::cout << "found call local" << std::endl;
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::min(std::stoi(instr->items[1]->value), 6); i++) {
						instr_items.push_back(f_l.caller_save[i]);
					}
					instr_l->gen = add_from_vec(&f_l, instr_items);
					if (printV) std::cout << "set gen" << std::endl;
					std::vector<size_t> new_item = add_items(&f_l, instr->items[0]);
					if (new_item.size() > 0)
						instr_l->gen.push_back(new_item[0]);
					instr_l->kill = add_from_vec(&f_l, f_l.caller_save);
					if (printV) std::cout << "set kill" << std::endl;
					break; }
				case call_runtime: { // special case
					if (printV) std::cout << "call runtime" << std::endl;
					std::vector<std::string> instr_items;
					int i;
					for (i=0; i<std::min(std::stoi(instr->items[1]->value), 6); i++) {
						instr_items.push_back(f_l.caller_save[i]);
					}
					instr_l->gen = add_from_vec(&f_l, instr_items);
					instr_l->kill = add_from_vec(&f_l, f_l.caller_save);
					// gen gets only the args dictated by N
					// kill gets all caller save
					break; }
				case load_stack: // dst gets written ONLY
					if (printV) std::cout << "load stack" << std::endl;
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
					if (f.instructions[i]->items[0]->value.compare("tensor-error") == 0) {
						f_l.instructions[i]->successors.push_back(-1); 
					} else {
						f_l.instructions[i]->successors.push_back(i+1);
					}
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


		if (printV) {
			int x=0;
			for (auto instr: f_l.instructions) {
				std::cout << "instruction " << x << std::endl;
				std::cout << "gen set: " << std::endl;
				for (auto elem : instr->gen) {
					std::cout << f_l.items_l[elem] << " ";
				}
				std::cout << "\nkill set: " << std::endl;
				for (auto elem : instr->kill) {
					std::cout << f_l.items_l[elem] << " ";
				}
				std::cout << "\n";
				x++;
			}
		}


		do {
			int i;
			f_l.isDirty = false;
			//for (i=0; i<f_l.instructions.size(); i++) {
			for (i=f_l.instructions.size()-1; i>=0; i--) {
				if (printV) std::cout << i << "starting out set " << i << std::endl;
				// OUT[i] = U(IN[s]) where s is all successors of i
				std::vector<size_t> new_set;
				if (printV) std::cout << i << "examining successors" << std::endl;
				for (int succ : f_l.instructions[i]->successors) {
					if (printV) std::cout << i << "successor " << succ << std::endl;
					if (succ >= 0) {
						if (printV) std::cout << i << "entering successor " << succ << " in (" << f_l.instructions[succ]->in.size() << " elements)" << std::endl;
						for (size_t elem : f_l.instructions[succ]->in) {
							if (std::find(new_set.begin(), new_set.end(), elem) == new_set.end()) { // elem not already in new_set
								if (printV) std::cout << i << "adding " << f_l.items_l[elem] << " from successor in" << std::endl;
								new_set.push_back(elem);
							}
								
						}
					} if (printV) std::cout << i << "finished successor " << succ << std::endl;
				}

				if (printV) std::cout << i << "checking for dirty" << std::endl;
				if (new_set.size() == f_l.instructions[i]->out.size()) { // make sure the for loop won't miss anything
					for (size_t elem : f_l.instructions[i]->out) {
						if (std::find(new_set.begin(), new_set.end(), elem) == new_set.end()) { // vectors are different
							if (printV) std::cout << i << "found mismatch: " << f_l.items_l[elem] << std::endl;
							for (size_t elem : new_set)
								f_l.instructions[i]->out.push_back(elem);
							f_l.isDirty = true;
						}
					}	
				} else {
					if (printV) std::cout << i << "old (" << f_l.instructions[i]->out.size() << ") and new (" << new_set.size() << ") not the same length" << std::endl;
					f_l.instructions[i]->out = new_set;
					f_l.isDirty = true;
				}
				if (printV) std::cout << i << "dirty is " << f_l.isDirty << std::endl;
				
				if (printV) std::cout << i << "starting in set " << i << std::endl;
				// IN[i] = GEN[i] U (OUT[i] - KILL[i])
				new_set = {};
				for (size_t elem : f_l.instructions[i]->gen) {
					if (std::find(new_set.begin(), new_set.end(), elem) == new_set.end()) {
						if (printV) std::cout << i << "adding " << f_l.items_l[elem] << " from gen" << std::endl;
						new_set.push_back(elem); 
					}
				}
				for (size_t elem : f_l.instructions[i]->out) {
					if (std::find(f_l.instructions[i]->kill.begin(), f_l.instructions[i]->kill.end(), elem) == f_l.instructions[i]->kill.end()) {
						if (std::find(new_set.begin(), new_set.end(), elem) == new_set.end()) {
							if (printV) std::cout << i << "adding " << f_l.items_l[elem] << " from out" << std::endl;
							new_set.push_back(elem);
						}
					}
				}

				if (printV) std::cout << i << "checking for dirty" << std::endl;
				if (new_set.size() == f_l.instructions[i]->in.size()) { // make sure the for loop won't miss anything
					for (size_t elem : f_l.instructions[i]->in) {
						if (std::find(new_set.begin(), new_set.end(), elem) == new_set.end()) { // vectors are different
							if (printV) std::cout << i << "found mismatch: " << f_l.items_l[elem] << std::endl;
							for (size_t elem : new_set)
								f_l.instructions[i]->in.push_back(elem);
							f_l.isDirty = true;
						}
					}	
				} else {
					if (printV) std::cout << i << "old (" << f_l.instructions[i]->in.size() << ") and new (" << new_set.size() << ") not the same length" << std::endl;
					f_l.instructions[i]->in = new_set;
					f_l.isDirty = true;
				}
				if (printV) std::cout << i << "dirty is " << f_l.isDirty << std::endl;
			}
			//std::cin.get();
		} while (f_l.isDirty);




		if (printV) {
			int x=0;
			for (auto instr: f_l.instructions) {
				std::cout << "instruction " << x << std::endl;
				std::cout << "in set: " << std::endl;
				for (auto elem : instr->in) {
					std::cout << f_l.items_l[elem] << " ";
				}
				std::cout << "\nout set: " << std::endl;
				for (auto elem : instr->out) {
					std::cout << f_l.items_l[elem] << " ";
				}
				std::cout << "\n";
				x++;
			}
		}




		return f_l;
	}

	void generate_inout_output(Function_l f_l) {
		std::cout << "(\n(in\n";
		int i;
		for (auto instr : f_l.instructions) {
			std::cout << "(";
			for (i=0; i<instr->in.size(); i++) {
				std::cout << f_l.items_l[instr->in[i]];
				if (i != instr->in.size()-1)
					std::cout << " ";
			}
			std::cout << ")\n";
		}
		std::cout << ")\n\n(out\n";
		for (auto instr : f_l.instructions) {
			std::cout << "(";
			for (i=0; i<instr->out.size(); i++) {
				std::cout << f_l.items_l[instr->out[i]];
				if (i != instr->out.size()-1)
					std::cout << " ";
			}
			std::cout << ")\n";
		}
		std::cout << ")\n\n)\n";
		return;
	}

	std::vector<size_t> add_from_vec(Function_l* f_l, std::vector<std::string>& strs) {
		std::vector<size_t> new_set;
		for (auto str : strs) {
			if (str.compare("rsp") != 0 && str.compare("") != 0) {
				if (printV) std::cout << "adding to add_from_vec: " << str << " (" << f_l->str_hash(str) << ")" << std::endl;
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
		if (str.compare("rsp") != 0 && str.compare("") != 0) {
			if (printV) std::cout << "adding to add_items: " << str << " (" << f_l->str_hash(str) << ")" << std::endl;
			new_set.push_back(f_l->str_hash(str));
			f_l->items_l[f_l->str_hash(str)] = str;
		}
		return new_set;
	}

}