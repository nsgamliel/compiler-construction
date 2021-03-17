#include <iostream>

#include <translations.h>

namespace IR {

	bool printTr = true;
	// if (printTr) std::cout << "" << std::endl;

	void translate(Function* f) {
		if (printTr) std::cout << "translating array init" << std::endl;
		translateArrayInitInstructions(f);
		if (printTr) std::cout << "translating array access" << std::endl;
		translateArrayAccessInstructions(f);
		if (printTr) std::cout << "translating tuple init" << std::endl;
		translateTupleInitInstructions(f);
		if (printTr) std::cout << "translating array access" << std::endl;
		translateTupleAccessInstructions(f);
		if (printTr) std::cout << "translating length instrs" << std::endl;
		translateLengthInstructions(f);

		return;
	}

	void translateLengthInstructions(Function* f) {
		for (auto bb : f->bbsTraced) {
			std::vector<Instruction*> newInstrs;
			for (auto instr : bb->instrs) {
				auto instrC = dynamic_cast<Instruction_length*>(instr);
				if (instrC) {
					if (printTr) std::cout << "found length instr" << std::endl;
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
					if (printTr) std::cout << "found array init" << std::endl;
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
						newInstrs.push_back(new Instruction_plus(instrC->dst, new Number((i+2) * 8), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
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
				if (instrC && !(instrC->isTup)) {
					if (printTr) std::cout << "found from array" << std::endl;
					newInstrs.push_back(new Instruction_mov(new Number(16), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					newInstrs.push_back(new Instruction_times(new Number(instrC->indices.size()), new Number(8), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					auto accumArg = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
					newInstrs.push_back(new Instruction_plus(new Variable(f->longestVar + std::to_string(f->numVarsAdded - 2)), new Variable(f->longestVar + std::to_string(f->numVarsAdded - 1)), accumArg));
					f->numVarsAdded++;
					// now add actual indices offset
					for (int i=0; i<instrC->indices.size(); i++) {
						auto indArg = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
						newInstrs.push_back(new Instruction_mov(instrC->indices[i], indArg));
						f->numVarsAdded++;
						for (int j=instrC->indices.size()-1; j>i; j--) {
							auto dimArg =  new Variable(f->longestVar + std::to_string(f->numVarsAdded));
							newInstrs.push_back(new Instruction_length(instrC->src, new Number(j), dimArg));
							newInstrs.push_back(new Instruction_rsh(dimArg, new Number(1), dimArg));
							newInstrs.push_back(new Instruction_times(indArg, dimArg, indArg));
							f->numVarsAdded++;
						}
						newInstrs.push_back(new Instruction_times(indArg, new Number(8), indArg));
						newInstrs.push_back(new Instruction_plus(accumArg, indArg, accumArg));
					}
					newInstrs.push_back(new Instruction_plus(accumArg, instrC->src, accumArg));
					newInstrs.push_back(new Instruction_load(accumArg, instrC->dst));
				} else if (instrC2 && !(instrC2->isTup)) {
					if (printTr) std::cout << "found to array" << std::endl;
					newInstrs.push_back(new Instruction_mov(new Number(16), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					newInstrs.push_back(new Instruction_times(new Number(instrC2->indices.size()), new Number(8), new Variable(f->longestVar + std::to_string(f->numVarsAdded))));
					f->numVarsAdded++;
					auto accumArg = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
					newInstrs.push_back(new Instruction_plus(new Variable(f->longestVar + std::to_string(f->numVarsAdded - 2)), new Variable(f->longestVar + std::to_string(f->numVarsAdded - 1)), accumArg));
					f->numVarsAdded++;
					// now add actual indices offset
					for (int i=0; i<instrC2->indices.size(); i++) {
						auto indArg = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
						newInstrs.push_back(new Instruction_mov(instrC2->indices[i], indArg));
						f->numVarsAdded++;
						for (int j=instrC2->indices.size()-1; j>i; j--) {
							auto dimArg =  new Variable(f->longestVar + std::to_string(f->numVarsAdded));
							newInstrs.push_back(new Instruction_length(instrC2->dst, new Number(j), dimArg));
							newInstrs.push_back(new Instruction_rsh(dimArg, new Number(1), dimArg));
							newInstrs.push_back(new Instruction_times(indArg, dimArg, indArg));
							f->numVarsAdded++;
						}
						newInstrs.push_back(new Instruction_times(indArg, new Number(8), indArg));
						newInstrs.push_back(new Instruction_plus(accumArg, indArg, accumArg));
					}
					newInstrs.push_back(new Instruction_plus(accumArg, instrC2->dst, accumArg));
					newInstrs.push_back(new Instruction_store(instrC2->src, accumArg));
				}	else {
					newInstrs.push_back(instr);
				}
			}
			bb->instrs = newInstrs;
		}
		return;
	}

	void translateTupleInitInstructions(Function* f) {
		for (auto bb : f->bbsTraced) {
			std::vector<Instruction*> newInstrs;
			for (auto instr : bb->instrs) {
				auto instrC = dynamic_cast<Instruction_tuple_init*>(instr);
				if (instrC) {
					if (printTr) std::cout << "found tuple init" << std::endl;
					newInstrs.push_back(new Instruction_call_allocate_assign(instrC->args, new Number(1), instrC->dst));
				} else {
					newInstrs.push_back(instr);
				}
			}
			bb->instrs = newInstrs;
		}
		return;
	}

	void translateTupleAccessInstructions(Function* f) {
		for (auto bb : f->bbsTraced) {
			std::vector<Instruction*> newInstrs;
			for (auto instr : bb->instrs) {
				auto instrC = dynamic_cast<Instruction_from_array*>(instr);
				auto instrC2 = dynamic_cast<Instruction_to_array*>(instr);
				if (instrC && (instrC->isTup)) {
					if (printTr) std::cout << "found from tuple" << std::endl;
					auto accumArg = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
					f->numVarsAdded++;
					newInstrs.push_back(new Instruction_times(instrC->indices[0], new Number(8), accumArg));
					newInstrs.push_back(new Instruction_plus(accumArg, new Number(8), accumArg));
					newInstrs.push_back(new Instruction_plus(accumArg, instrC->src, accumArg));
					newInstrs.push_back(new Instruction_load(accumArg, instrC->dst));
				} else if (instrC2 && (instrC2->isTup)) {
					if (printTr) std::cout << "found to tuple" << std::endl;
					auto accumArg = new Variable(f->longestVar + std::to_string(f->numVarsAdded));
					f->numVarsAdded++;
					newInstrs.push_back(new Instruction_times(instrC2->indices[0], new Number(8), accumArg));
					newInstrs.push_back(new Instruction_plus(accumArg, new Number(8), accumArg));
					newInstrs.push_back(new Instruction_plus(accumArg, instrC2->dst, accumArg));
					newInstrs.push_back(new Instruction_store(instrC2->src, accumArg));
				}	else {
					newInstrs.push_back(instr);
				}
			}
			bb->instrs = newInstrs;
		}
		return;
	}

}