#pragma once

#include <map>
#include <string>
#include <vector>

#include <L2.h>

namespace L2 {

	struct Instruction_l {
		// store hashed values of variable strings
		std::vector<size_t> gen;
		std::vector<size_t> kill;
		std::vector<size_t> in;
		std::vector<size_t> out;
		// only used in special case
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

	void generate_inout_output(Fucntion_l f_l, char* fileName);

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

/*

liveness analysis plan:

1 function takes in program p
2 figure out gen and kill sets - unique to each instruction or set of
3 while this is going on, flag instructions with multiple successors
4 in and out while isDirty

data representation
- each instruction needs a gen, kill, in, out, successors
	- successors will only have contents if special case
- Instruction_l and Function_l structs
- Instruction_l
	- gen, kill, in, out -> item pointer vectors
		- will need switch case to determine which member to pull name from
	- successors -> integer vector
- Function_l
	- isDirty bool
	- instruction_l pointer vector

- design problem:
	- how to set up the items in each vector so that it is easy to check for their existence? ie for duplicates or for in/out sets
	- consider defining new item:
		- type
		- integer unique id -- how to generate this consistently? -> hash
		- string representation

	- instead, instructions contain vectors of size_t hashes and look items up in a master dictionary
		- only need to perform the lookup when generating the in/out sets output file
		- use std::map<>

*/
