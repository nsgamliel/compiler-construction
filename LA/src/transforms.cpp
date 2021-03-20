#include <iostream>
#include <vector>

#include <transforms.h>

namespace LA {

	bool printTr = false;
	// if (printTr) std::cout << "" << std::endl;

	void transform(Function* f) {
		if (printTr) std::cout << "---entering encodedecode" << std::endl;
		encodeDecode(f);
		if (printTr) std::cout << "---entering check alloc access" << std::endl;
		checkAllocationAccess(f);
		if (printTr) std::cout << "---entering buildbasic" << std::endl;
		buildBasicBlocks(f);
		if (printTr) std::cout << "---leaving transform" << std::endl;
		return;
	}

	void encodeDecode(Function* f) {
		std::vector<Instruction*> newInstrs;
		for (auto instr : f->instrs) {
			auto newI = dynamic_cast<Instruction_op*>(instr);
			if (newI) {
				Item* left;
				Item* right;
				if (!(dynamic_cast<Number*>(newI->left))) {
					left = new Variable(f->longestVar + std::to_string(f->numSub));
					newInstrs.push_back(new Instruction_define_var("int64", new Variable(f->longestVar + std::to_string(f->numSub))));
					newInstrs.push_back(new Instruction_rsh(newI->left, new Number(1), new Variable(f->longestVar + std::to_string(f->numSub))));
					f->numSub++;
				} else { left = newI->left; }
				if (!(dynamic_cast<Number*>(newI->right))) {
					right = new Variable(f->longestVar + std::to_string(f->numSub));
					newInstrs.push_back(new Instruction_define_var("int64", new Variable(f->longestVar + std::to_string(f->numSub))));
					newInstrs.push_back(new Instruction_rsh(newI->right, new Number(1), new Variable(f->longestVar + std::to_string(f->numSub))));
					f->numSub++;
				} else { right = newI->right; }
				//std::cout << "asdfasdfasdf instr opcode: " << std::to_string(newI->opcode) << std::endl;
				switch (newI->getOpcode()) {
					case 0:  newInstrs.push_back(new Instruction_plus(left, right, newI->dst)); break;
					case 1:  newInstrs.push_back(new Instruction_minus(left, right, newI->dst)); break;
					case 2:  newInstrs.push_back(new Instruction_times(left, right, newI->dst)); break;
					case 3:  newInstrs.push_back(new Instruction_and(left, right, newI->dst)); break;
					case 4:  newInstrs.push_back(new Instruction_lsh(left, right, newI->dst)); break;
					case 5:  newInstrs.push_back(new Instruction_rsh(left, right, newI->dst)); break;
					case 6:  newInstrs.push_back(new Instruction_eq(left, right, newI->dst)); break;
					case 7:  newInstrs.push_back(new Instruction_le(left, right, newI->dst)); break;
					case 8:  newInstrs.push_back(new Instruction_ge(left, right, newI->dst)); break;
					case 9:  newInstrs.push_back(new Instruction_less(left, right, newI->dst)); break;
					case 10: newInstrs.push_back(new Instruction_greater(left, right, newI->dst)); break;
				}
				newInstrs.push_back(new Instruction_lsh(newI->dst, new Number(1), newI->dst));
				newInstrs.push_back(new Instruction_plus(newI->dst, new Number(1), newI->dst));
				continue;
			}
			auto newI2 = dynamic_cast<Instruction_from_array*>(instr);
			if (newI2) {
				std::vector<Item*> newInds;
				for (auto ind : newI2->indices) {
					if (!(dynamic_cast<Number*>(ind))) {
						auto newVar = new Variable(f->longestVar + std::to_string(f->numSub));
						newInstrs.push_back(new Instruction_define_var("int64", newVar));
						newInstrs.push_back(new Instruction_rsh(ind, new Number(1), newVar));
						newInds.push_back(newVar);
						f->numSub++;
					} else { newInds.push_back(ind); }
				}
				newInstrs.push_back(new Instruction_from_array(newI2->lineNumber, newI2->isTup, newI2->src, newInds, newI2->dst));
				continue;
			}
			auto newI3 = dynamic_cast<Instruction_to_array*>(instr);
			if (newI3) {
				std::vector<Item*> newInds;
				for (auto ind : newI3->indices) {
					if (!(dynamic_cast<Number*>(ind))) {
						auto newVar = new Variable(f->longestVar + std::to_string(f->numSub));
						newInstrs.push_back(new Instruction_define_var("int64", newVar));
						newInstrs.push_back(new Instruction_rsh(ind, new Number(1), newVar));
						newInds.push_back(newVar);
						f->numSub++;
					} else { newInds.push_back(ind); }
				}
				auto srcVal = dynamic_cast<Number*>(newI3->src);
				if (srcVal) {
					newInstrs.push_back(new Instruction_to_array(newI3->lineNumber, newI3->isTup, new Number(2*srcVal->value + 1), newInds, newI3->dst));
					continue;
				}
				newInstrs.push_back(new Instruction_to_array(newI3->lineNumber, newI3->isTup, newI3->src, newInds, newI3->dst));
				continue;
			}
			auto newI4 = dynamic_cast<Instruction_cond_branch*>(instr);
			if (newI4) {
				auto conC = dynamic_cast<Variable*>(newI4->cond);
				Item* newCond;
				if (conC && conC->type.compare("int64") == 0) {
					newCond = new Variable(f->longestVar + std::to_string(f->numSub));
					newInstrs.push_back(new Instruction_define_var("int64", new Variable(f->longestVar + std::to_string(f->numSub))));
					newInstrs.push_back(new Instruction_rsh(conC, new Number(1), new Variable(f->longestVar + std::to_string(f->numSub))));
					f->numSub++;
				} else { newCond = newI4->cond; }
				newInstrs.push_back(new Instruction_cond_branch(newCond, newI4->labelTrue, newI4->labelFalse));
				continue;
			}
			auto newI5 = dynamic_cast<Instruction_length*>(instr);
			if (newI5) {
				auto dimC = dynamic_cast<Variable*>(newI5->dim);
				Item* newDim;
				if (dimC && dimC->type.compare("int64") == 0) {
					newDim = new Variable(f->longestVar + std::to_string(f->numSub));
					f->numSub++;
					newInstrs.push_back(new Instruction_define_var("int64", new Variable(f->longestVar + std::to_string(f->numSub))));
					newInstrs.push_back(new Instruction_rsh(dimC, new Number(1), new Variable(f->longestVar + std::to_string(f->numSub))));
				} else { newDim = newI5->dim; }
				newInstrs.push_back(new Instruction_length(newI5->src, newDim, newI5->dst));
				continue;
			}
			// add one more for basic move of number to variable to encode that number is encoded
			auto newI6 = dynamic_cast<Instruction_mov*>(instr);
			if (newI6) {
				auto srcC = dynamic_cast<Number*>(newI6->src);
				Item* newSrc;
				if (srcC) {
					newSrc = new Number(2*srcC->value + 1);
				} else {
					newSrc = newI6->src;
				}
				newInstrs.push_back(new Instruction_mov(newSrc, newI6->dst));
				continue;
			}
			/*auto newI7 = dynamic_cast<Instruction_call_print*>(instr);
			if (newI7) {
				auto argC = dynamic_cast<Number*>(newI7->arg);
				if (argC) {
					newInstrs.push_back(new Instruction_call_print(new Number(2*argC->value + 1)));
					continue;
				}
			}*/
			// otherwise, add the current instruction
			newInstrs.push_back(instr);
		}
		f->instrs = newInstrs;
		return;
	} 

	void checkAllocationAccess(Function* f) {
		std::vector<Instruction*> newInstrs;
		for (auto instr : f->instrs) {
			auto instrC = dynamic_cast<Instruction_from_array*>(instr);
			if (instrC) {
				if (printTr) std::cout << "found from array" << std::endl;
				// line number
				auto lnvar = new Variable(f->longestVar + std::to_string(f->numSub) + "_ln");
				std::vector<Item*> teArg;
				teArg.push_back(lnvar);
				f->numSub++;
				newInstrs.push_back(new Instruction_define_var("int64", lnvar));
				newInstrs.push_back(new Instruction_mov(new Number(instrC->lineNumber), lnvar));
				// allocation check
				auto alvar = new Variable(f->longestVar + std::to_string(f->numSub));
				auto isAl = new Label(f->longestVar + std::to_string(f->numSub+1) + "_tl");
				auto notAl = new Label(f->longestVar + std::to_string(f->numSub+2) + "_fl");
				f->numSub += 3;
				newInstrs.push_back(new Instruction_define_var("int64", alvar));
				newInstrs.push_back(new Instruction_eq(instrC->src, new Number(0), alvar));
				newInstrs.push_back(new Instruction_cond_branch(alvar, notAl, isAl));
				newInstrs.push_back(new Instruction_label(notAl));
				newInstrs.push_back(new Instruction_call_tensor_error(teArg));
				newInstrs.push_back(new Instruction_label(isAl));
				// indices check
				Variable* dimVar;
				Variable* dimVarDec;
				Variable* checkVar;
				Label* inBounds;
				Label* outBounds;
				Variable* indEnc;
				// first check for case of single-dim array
				if (instrC->indices.size() == 1  && !instrC->isTup) {
					// load dim length
					dimVar = new Variable(f->longestVar + std::to_string(f->numSub) + "_dim");
					dimVarDec = new Variable(f->longestVar + std::to_string(f->numSub) + "_dimDec");
					f->numSub++;
					newInstrs.push_back(new Instruction_define_var("int64", dimVar));
					newInstrs.push_back(new Instruction_length(instrC->src, new Number(0), dimVar));
					newInstrs.push_back(new Instruction_define_var("int64", dimVarDec));
					newInstrs.push_back(new Instruction_rsh(dimVar, new Number(1), dimVarDec));
					// access and compare dim
					checkVar = new Variable(f->longestVar + std::to_string(f->numSub));
					inBounds = new Label(f->longestVar + std::to_string(f->numSub+1) + "_tl");
					outBounds = new Label(f->longestVar + std::to_string(f->numSub+2) + "_fl");
					f->numSub += 3;
					newInstrs.push_back(new Instruction_less(instrC->indices[0], dimVarDec, checkVar));
					newInstrs.push_back(new Instruction_cond_branch(checkVar, inBounds, outBounds));
					newInstrs.push_back(new Instruction_label(outBounds));
					teArg.clear();
					teArg.push_back(lnvar);
					teArg.push_back(dimVar);
					indEnc = new Variable(f->longestVar + std::to_string(f->numSub) + "_indEnc");
					f->numSub++;
					newInstrs.push_back(new Instruction_define_var("int64", indEnc));
					newInstrs.push_back(new Instruction_lsh(instrC->indices[0], new Number(1), indEnc));
					newInstrs.push_back(new Instruction_plus(indEnc, new Number(1), indEnc));
					teArg.push_back(indEnc);
					newInstrs.push_back(new Instruction_call_tensor_error(teArg));
					newInstrs.push_back(new Instruction_label(inBounds));
				} else if (!instrC->isTup) {
					for (int i=0; i<instrC->indices.size(); i++) {
						// load dim length
						dimVar = new Variable(f->longestVar + std::to_string(f->numSub) + "_dim");
						dimVarDec = new Variable(f->longestVar + std::to_string(f->numSub) + "_dimDec");
						f->numSub++;
						newInstrs.push_back(new Instruction_define_var("int64", dimVar));
						newInstrs.push_back(new Instruction_length(instrC->src, new Number(i), dimVar));
						newInstrs.push_back(new Instruction_define_var("int64", dimVarDec));
						newInstrs.push_back(new Instruction_rsh(dimVar, new Number(1), dimVarDec));
						// access and compare dim
						checkVar = new Variable(f->longestVar + std::to_string(f->numSub));
						inBounds = new Label(f->longestVar + std::to_string(f->numSub+1) + "_tl");
						outBounds = new Label(f->longestVar + std::to_string(f->numSub+2) + "_fl");
						f->numSub += 3;
						newInstrs.push_back(new Instruction_less(instrC->indices[i], dimVarDec, checkVar));
						newInstrs.push_back(new Instruction_cond_branch(checkVar, inBounds, outBounds));
						newInstrs.push_back(new Instruction_label(outBounds));
						teArg.clear();
						teArg.push_back(lnvar);
						teArg.push_back(new Number(2*i + 1));
						teArg.push_back(dimVar);
						auto oindEnc = new Variable(f->longestVar + std::to_string(f->numSub) + "_indEnc");
						f->numSub++;
						newInstrs.push_back(new Instruction_define_var("int64", oindEnc));
						newInstrs.push_back(new Instruction_lsh(instrC->indices[i], new Number(1), oindEnc));
						newInstrs.push_back(new Instruction_plus(oindEnc, new Number(1), oindEnc));
						teArg.push_back(oindEnc);
						newInstrs.push_back(new Instruction_call_tensor_error(teArg));
						newInstrs.push_back(new Instruction_label(inBounds));
					}
				}
				newInstrs.push_back(instrC);
				continue;
			}
			auto instrC2 = dynamic_cast<Instruction_to_array*>(instr);
			if (instrC2) {
				if (printTr) std::cout << "found to array" << std::endl;
				// line number
				if (printTr) std::cout << "setting up line number" << std::endl;
				auto lnvar2 = new Variable(f->longestVar + std::to_string(f->numSub) + "_ln");
				std::vector<Item*> teArg2;
				teArg2.push_back(lnvar2);
				f->numSub++;
				newInstrs.push_back(new Instruction_define_var("int64", lnvar2));
				newInstrs.push_back(new Instruction_mov(new Number(instrC2->lineNumber), lnvar2));
				// allocation check
				if (printTr) std::cout << "allocation check" << std::endl;
				auto alvar2 = new Variable(f->longestVar + std::to_string(f->numSub));
				auto isAl2 = new Label(f->longestVar + std::to_string(f->numSub+1) + "_tl");
				auto notAl2 = new Label(f->longestVar + std::to_string(f->numSub+2) + "_fl");
				f->numSub += 3;
				newInstrs.push_back(new Instruction_define_var("int64", alvar2));
				newInstrs.push_back(new Instruction_eq(instrC2->dst, new Number(0), alvar2));
				newInstrs.push_back(new Instruction_cond_branch(alvar2, notAl2, isAl2));
				newInstrs.push_back(new Instruction_label(notAl2));
				newInstrs.push_back(new Instruction_call_tensor_error(teArg2));
				newInstrs.push_back(new Instruction_label(isAl2));
				// indices check
				Variable* dimVar2;
				Variable* dimVarDec2;
				Variable* checkVar2;
				Label* inBounds2;
				Label* outBounds2;
				Variable* indEnc2;
				// first check for case of single-dim array
				if (instrC2->indices.size() == 1 && !instrC2->isTup) {
					if (printTr) std::cout << "is single-dim array" << std::endl;
					// load dim length
					dimVar2 = new Variable(f->longestVar + std::to_string(f->numSub) + "_dim");
					dimVarDec2 = new Variable(f->longestVar + std::to_string(f->numSub) + "_dimDec");
					f->numSub++;
					newInstrs.push_back(new Instruction_define_var("int64", dimVar2));
					newInstrs.push_back(new Instruction_length(instrC2->dst, new Number(0), dimVar2));
					newInstrs.push_back(new Instruction_define_var("int64", dimVarDec2));
					newInstrs.push_back(new Instruction_rsh(dimVar2, new Number(1), dimVarDec2));
					// access and compare dim
					checkVar2 = new Variable(f->longestVar + std::to_string(f->numSub));
					inBounds2 = new Label(f->longestVar + std::to_string(f->numSub+1) + "_tl");
					outBounds2 = new Label(f->longestVar + std::to_string(f->numSub+2) + "_fl");
					f->numSub += 3;
					newInstrs.push_back(new Instruction_less(instrC2->indices[0], dimVarDec2, checkVar2));
					newInstrs.push_back(new Instruction_cond_branch(checkVar2, inBounds2, outBounds2));
					newInstrs.push_back(new Instruction_label(outBounds2));
					teArg2.clear();
					teArg2.push_back(lnvar2);
					teArg2.push_back(dimVar2);
					indEnc2 = new Variable(f->longestVar + std::to_string(f->numSub) + "_indEnc");
					f->numSub++;
					newInstrs.push_back(new Instruction_define_var("int64", indEnc2));
					newInstrs.push_back(new Instruction_lsh(instrC2->indices[0], new Number(1), indEnc2));
					newInstrs.push_back(new Instruction_plus(indEnc2, new Number(1), indEnc2));
					teArg2.push_back(indEnc2);
					newInstrs.push_back(new Instruction_call_tensor_error(teArg2));
					newInstrs.push_back(new Instruction_label(inBounds2));
				} else if (!instrC2->isTup) {
					if (printTr) std::cout << "multiple dims" << std::endl;
					for (int i=0; i<instrC2->indices.size(); i++) {
						// load dim length
						dimVar2 = new Variable(f->longestVar + std::to_string(f->numSub) + "_dim");
						dimVarDec2 = new Variable(f->longestVar + std::to_string(f->numSub) + "_dimDec");
						f->numSub++;
						newInstrs.push_back(new Instruction_define_var("int64", dimVar2));
						newInstrs.push_back(new Instruction_length(instrC2->dst, new Number(i), dimVar2));
						newInstrs.push_back(new Instruction_define_var("int64", dimVarDec2));
						newInstrs.push_back(new Instruction_rsh(dimVar2, new Number(1), dimVarDec2));
						// access and compare dim
						checkVar2 = new Variable(f->longestVar + std::to_string(f->numSub));
						inBounds2 = new Label(f->longestVar + std::to_string(f->numSub+1) + "_tl");
						outBounds2 = new Label(f->longestVar + std::to_string(f->numSub+2) + "_fl");
						f->numSub += 3;
						newInstrs.push_back(new Instruction_less(instrC2->indices[i], dimVarDec2, checkVar2));
						newInstrs.push_back(new Instruction_cond_branch(checkVar2, inBounds2, outBounds2));
						newInstrs.push_back(new Instruction_label(outBounds2));
						teArg2.clear();
						teArg2.push_back(lnvar2);
						teArg2.push_back(new Number(2*i + 1));
						teArg2.push_back(dimVar2);
						auto oindEnc2 = new Variable(f->longestVar + std::to_string(f->numSub) + "_indEnc");
						f->numSub++;
						newInstrs.push_back(new Instruction_define_var("int64", oindEnc2));
						newInstrs.push_back(new Instruction_lsh(instrC2->indices[i], new Number(1), oindEnc2));
						newInstrs.push_back(new Instruction_plus(oindEnc2, new Number(1), oindEnc2));
						teArg2.push_back(oindEnc2);
						newInstrs.push_back(new Instruction_call_tensor_error(teArg2));
						newInstrs.push_back(new Instruction_label(inBounds2));
					}
				}
				newInstrs.push_back(instrC2);
				continue;
			}
			// else, add the instruction and move on
			newInstrs.push_back(instr);
		}
		f->instrs = newInstrs;
		return;
	}

	void buildBasicBlocks(Function* f) {
		std::vector<Instruction*> newInstrs;
		bool startBB = true;
		for (auto instr : f->instrs) {
			if (startBB) {
				if (!(dynamic_cast<Instruction_label*>(instr))) {
					newInstrs.push_back(new Instruction_label(new Label(f->longestLabel + std::to_string(f->numSub))));
					f->numSub++;
				}
				startBB = false;
			} else if (dynamic_cast<Instruction_label*>(instr)) {
				newInstrs.push_back(new Instruction_branch(dynamic_cast<Instruction_label*>(instr)->label));
			}
			newInstrs.push_back(instr);
			if (dynamic_cast<Instruction_branch*>(instr) || 
			    dynamic_cast<Instruction_cond_branch*>(instr) || 
					dynamic_cast<Instruction_return*>(instr) ||
					dynamic_cast<Instruction_return_val*>(instr)) {
				startBB = true;
			}
		}
		if (!startBB) {
			if (f->retType.compare("void") == 0) {
				newInstrs.push_back(new Instruction_return());
			} else {
				newInstrs.push_back(new Instruction_return_val(new Number(0)));
			}
		}
		f->instrs = newInstrs;
		return;
	}

}