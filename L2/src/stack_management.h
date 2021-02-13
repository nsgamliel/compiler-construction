#pragma once

#include <L2.h>

/* TODO
 * 
 * figure out how much stack space f currently uses for local vars
 * store used callee-save registers with appropriate byte offsets
 * create a cumulative tally of local vars used to be able to generate L1
 * translate stack-arg to be the stack space above locals
 * 
 * note: register-allocation will likely invoke everything through callee-save storing
 *       if this manages number of locals properly, stack-arg translation can just be a 
 *       member of the stack-arg instruction class that takes in a Function* and gives back
 *       the appropriate load instruction pointer
 *
 * update: based on piazza this will assume no explicit stack usage
 * 
 */

namespace L2 {

	//int calculate_stack_usage(Function* f);

	void save_to_stack(Function* f, std::vector<Variable*> regs);

	void convert_stack_args(Function* f);

}