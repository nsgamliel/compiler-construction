#include <translations.h>

namespace IR {

	void translate(Function* f) {
		translateArrayInitInstructions(f);
		translateLengthInstructions(f);

		return;
	}

	void translateLengthInstructions(Function* f) {
		for (auto bb : f->bbsTraced) {
			std::vector<Instruction*> newInstrs;
			for (auto instr : bb->instrs) {
				auto instrC = dynamic_cast<Instruction_length*>(instr);
				if (instrC) {
					newInstrs.push_back(new Instruction_times(instrC->dim, new Number(8), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					newInstrs.push_back(new Instruction_plus(new Variable(f->longestVar + std::to_string(f->numVarsAdded - 1)), new Number(16), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					newInstrs.push_back(new Instruction_plus(instrC->src, new Variable(f->longestVar + std::to_string(f->numVarsAdded - 1)), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					newInstrs.push_back(new Instruction_load(new Variable(f->longestVar + std::to_string(f->numVarsAdded)), instrC->dst));
					f->numVarsAdded++;
				} else {
					newInstrs.push_back(instr);
				}
			}
			bb->instrs = newInstrs;
		}
		return;
	}

	void translateArrayInitInstructions(Function* f) {
		for (auto bb : f->bbsTraced) {
			std::vector<Instruction*> newInstrs;
			for (auto instr : bb->instrs) {
				auto instrC = dynamic_cast<Instruction_array_init*>(instr);
				if (instrC) {
					int prodAccum = f->numVarsAdded;
					f->numVarsAdded++;
					auto accumVar = new Variable(f->longestVar + std::to_string(prodAccum));
					newInstrs.push_back(new Instruction_mov(new Number(1), accumVar));
					for (auto arg : instrC->args) {
						auto argVar = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
						newInstrs.push_back(new Instruction_rsh(arg, new Number(1), argVar));
						newInstrs.push_back(new Instruction_times(accumVar, argVar, accumVar));
						f->numVarsAdded++;
					}
					newInstrs.push_back(new Instruction_plus(accumVar, new Number(instrC->args.size() + 1), accumVar));
					newInstrs.push_back(new Instruction_lsh(accumVar, new Number(1), accumVar));
					newInstrs.push_back(new Instruction_plus(accumVar, new Number(1), accumVar));
					newInstrs.push_back(new Instruction_call_allocate_assign(accumVar, new Number(1), instrC->dst));
					newInstrs.push_back(new Instruction_plus(instrC->dst, new Number(8), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					newInstrs.push_back(new Instruction_store(new Number(2*instrC->args.size() + 1), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					for (int i=0; i<instrC->args.size(); i++) {
						newInstrs.push_back(new Instruction_plus(instrC->dst, new Number((i+1) * 8), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
						newInstrs.push_back(new Instruction_store(instrC->args[i], new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
						f->numVarsAdded++;
					}
				} else {
					newInstrs.push_back(instr);
				}
			}
			bb->instrs = newInstrs;
		}
		return;
	}

	void translateArrayAccessInstructions(Function* f) {
		for (auto bb : f->bbsTraced) {
			std::vector<Instruction*> newInstrs;
			for (auto instr : bb->instrs) {
				auto instrC = dynamic_cast<Instruction_from_array*>(instr);
				auto instrC2 = dynamic_cast<Instruction_to_array*>(instr);
				if (instrC) {
					newInstrs.push_back(new Instruction_mov(new Number(16), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					newInstrs.push_back(new Instruction_times(new Number(instrC->indices.size()), new Number(8), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					auto accumArg = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
					newInstrs.push_back(new Instruction_plus(new Variable(f->longestVar + std::to_string(f->numVarsAdded - 2)), new Variable(f->longestVar + std::to_string(f->numVarsAdded - 1)), accumArg));
					f->numVarsAdded++;
					// now add actual indices offset
					for (int i=0; i<instrC->indices.size(); i++) {
						
					}

				} else if (instrC2) {
					
				}	else {
					newInstrs.push_back(instr);
				}
			}
			bb->instrs = newInstrs;
		}
		return;
	}

}