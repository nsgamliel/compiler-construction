#pragma once

#include <map>
#include <string>
#include <vector>

#include <L2.h>

namespace L2 {

	struct Instruction_l {
		// store hashed values of variable strings
		L2::Opcode op;
		std::vector<size_t> gen;
		std::vector<size_t> kill;
		std::vector<size_t> in;
		std::vector<size_t> out;
		std::vector<int> successors;
	};

	struct Function_l {
		bool isDirty;
		std::hash<std::string> str_hash;
		std::vector<std::string> callee_save = {"r12", "r13", "r14", "r15", "rbp", "rbx"};
		std::vector<std::string> caller_save = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", "r10", "r11", "rax"};
		std::vector<Instruction_l*> instructions;
		std::map<size_t, std::string> items_l;
	};

	Function_l generate_liveness(L2::Program p);

	void generate_liveness_output(Function_l f_l);

	// helper functions
	Function_l gen_kill(L2::Function& f);
	Function_l find_successors(Function_l f_l, L2::Function& f);
	Function_l in_out(Function_l fl);


	// add items to the vector corresponding to the location argument
	// also adds the items to the function's items dictionary
	// prevents rsp from being added anywhere
	std::vector<size_t> add_from_vec(Function_l* f_l, std::vector<std::string>& strs);
	std::vector<size_t> add_items(Function_l* f_l, L2::Item* item);

}
