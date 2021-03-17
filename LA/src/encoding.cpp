#include <iostream>
#include <vector>

#include <encoding.h>

namespace LA {

	void encodeDecode(Function* f) {
		std::vector<Instruction*> newInstrs;
		for (auto instr : f->instrs) {
			auto newI = dynamic_cast<Instruction_op*>(instr);
			if (newI) {
				newInstrs.push_back(new Instruction_define_var("int64", new Variable(f->longestVar + std::to_string(f->numSub))));
				newInstrs.push_back(new Instruction_rsh(newI->left, new Number(1), new Variable(f->longestVar + std::to_string(f->numSub))));
				f->numSub++;
				newInstrs.push_back(new Instruction_define_var("int64", new Variable(f->longestVar + std::to_string(f->numSub))));
				newInstrs.push_back(new Instruction_rsh(newI->right, new Number(1), new Variable(f->longestVar + std::to_string(f->numSub))));
				f->numSub++;
				switch (newI->opcode) {
					case 0:  newInstrs.push_back(new Instruction_plus(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 1:  newInstrs.push_back(new Instruction_minus(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 2:  newInstrs.push_back(new Instruction_times(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 3:  newInstrs.push_back(new Instruction_and(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 4:  newInstrs.push_back(new Instruction_lsh(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 5:  newInstrs.push_back(new Instruction_rsh(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 6:  newInstrs.push_back(new Instruction_eq(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 7:  newInstrs.push_back(new Instruction_le(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 8:  newInstrs.push_back(new Instruction_ge(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 9:  newInstrs.push_back(new Instruction_less(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
					case 10: newInstrs.push_back(new Instruction_greater(new Variable(f->longestVar + std::to_string(f->numSub-2)), new Variable(f->longestVar + std::to_string(f->numSub-1)), newI->dst)); break;
				}
				newInstrs.push_back(new Instruction_lsh(newI->dst, new Number(1), newI->dst));
				newInstrs.push_back(new Instruction_plus(newI->dst, new Number(1), newI->dst));
				continue;
			}
			auto newI2 = dynamic_cast<Instruction_from_array*>(instr);
			if (newI2) {
				// for index in indices, shift right and assign to new var, add var to vector of new indices, put together
			}

			// to array

			// cond branch

			// length
		}
	}

}