#include <tiler.h>

#include <iostream>

namespace L3 {

	/*
	 * match methods
	 */

	bool Tile_return::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_return*>(i->instr)) return true;
		return false;
	}

	bool Tile_return_val::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_return_val*>(i->instr)) return true;
		return false;
	}

	bool Tile_mov::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_mov*>(i->instr)) return true;
		return false;
	}

	bool Tile_plus::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_plus*>(i->instr)) return true;
		return false;
	}

	bool Tile_minus::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_minus*>(i->instr)) return true;
		return false;
	}

	bool Tile_times::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_times*>(i->instr)) return true;
		return false;
	}

	bool Tile_and::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_and*>(i->instr)) return true;
		return false;
	}

	bool Tile_lsh::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_lsh*>(i->instr)) return true;
		return false;
	}

	bool Tile_rsh::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_rsh*>(i->instr)) return true;
		return false;
	}

	bool Tile_eq::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_eq*>(i->instr)) return true;
		return false;
	}

	bool Tile_le::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_le*>(i->instr)) return true;
		return false;
	}

	bool Tile_ge::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_ge*>(i->instr)) return true;
		return false;
	}

	bool Tile_less::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_less*>(i->instr)) return true;
		return false;
	}

	bool Tile_greater::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_greater*>(i->instr)) return true;
		return false;
	}

	bool Tile_load::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_load*>(i->instr)) return true;
		return false;
	}

	bool Tile_store::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_store*>(i->instr)) return true;
		return false;
	}

	bool Tile_label::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_label*>(i->instr)) return true;
		return false;
	}

	bool Tile_branch::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_branch*>(i->instr)) return true;
		return false;
	}

	bool Tile_cond_branch::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_cond_branch*>(i->instr)) return true;
		return false;
	}

	bool Tile_call::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call*>(i->instr)) return true;
		return false;
	}

	bool Tile_call_print::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_print*>(i->instr)) return true;
		return false;
	}

	bool Tile_call_allocate::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_allocate*>(i->instr)) return true;
		return false;
	}

	bool Tile_call_input::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_input*>(i->instr)) return true;
		return false;
	}

	bool Tile_call_tensor_error::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_tensor_error*>(i->instr)) return true;
		return false;
	}

	bool Tile_call_assign::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_assign*>(i->instr)) return true;
		return false;
	}

	bool Tile_call_allocate_assign::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_allocate_assign*>(i->instr)) return true;
		return false;
	}

	bool Tile_call_input_assign::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_input_assign*>(i->instr)) return true;
		return false;
	}


	/*
	 * accept methods	
	 */

	/*void Tile_return::accept(TileVisitor* tv) { return; }
	void Tile_return_val::accept(TileVisitor* tv) { return; }
	void Tile_mov::accept(TileVisitor* tv) { return; }
	void Tile_plus::accept(TileVisitor* tv) { return; }
	void Tile_minus::accept(TileVisitor* tv) { return; }
	void Tile_times::accept(TileVisitor* tv) { return; }
	void Tile_and::accept(TileVisitor* tv) { return; }
	void Tile_lsh::accept(TileVisitor* tv) { return; }
	void Tile_rsh::accept(TileVisitor* tv) { return; }
	void Tile_eq::accept(TileVisitor* tv) { return; }
	void Tile_le::accept(TileVisitor* tv) { return; }
	void Tile_ge::accept(TileVisitor* tv) { return; }
	void Tile_less::accept(TileVisitor* tv) { return; }
	void Tile_greater::accept(TileVisitor* tv) { return; }
	void Tile_load::accept(TileVisitor* tv) { return; }
	void Tile_store::accept(TileVisitor* tv) { return; }
	void Tile_label::accept(TileVisitor* tv) { return; }
	void Tile_branch::accept(TileVisitor* tv) { return; }
	void Tile_cond_branch::accept(TileVisitor* tv) { return; }
	void Tile_call::accept(TileVisitor* tv) { return; }
	void Tile_call_print::accept(TileVisitor* tv) { return; }
	void Tile_call_allocate::accept(TileVisitor* tv) { return; }
	void Tile_call_input::accept(TileVisitor* tv) { return; }
	void Tile_call_tensor_error::accept(TileVisitor* tv) { return; }
	void Tile_call_assign::accept(TileVisitor* tv) { return; }
	void Tile_call_allocate_assign::accept(TileVisitor* tv) { return; }
	void Tile_call_input_assign::accept(TileVisitor* tv) { return; }*/

	/*
	 * clone methods
	 */

	Tile_return* Tile_return::clone() { return new Tile_return(); }
	Tile_return_val* Tile_return_val::clone() { return new Tile_return_val(); }
	Tile_mov* Tile_mov::clone() { return new Tile_mov(); }
	Tile_plus* Tile_plus::clone() { return new Tile_plus(); }
	Tile_minus* Tile_minus::clone() { return new Tile_minus(); }
	Tile_times* Tile_times::clone() { return new Tile_times(); }
	Tile_and* Tile_and::clone() { return new Tile_and(); }
	Tile_lsh* Tile_lsh::clone() { return new Tile_lsh(); }
	Tile_rsh* Tile_rsh::clone() { return new Tile_rsh(); }
	Tile_eq* Tile_eq::clone() { return new Tile_eq(); }
	Tile_le* Tile_le::clone() { return new Tile_le(); }
	Tile_ge* Tile_ge::clone() { return new Tile_ge(); }
	Tile_less* Tile_less::clone() { return new Tile_less(); }
	Tile_greater* Tile_greater::clone() { return new Tile_greater(); }
	Tile_load* Tile_load::clone() { return new Tile_load(); }
	Tile_store* Tile_store::clone() { return new Tile_store(); }
	Tile_label* Tile_label::clone() { return new Tile_label(); }
	Tile_branch* Tile_branch::clone() { return new Tile_branch(); }
	Tile_cond_branch* Tile_cond_branch::clone() { return new Tile_cond_branch(); }
	Tile_call* Tile_call::clone() { return new Tile_call(); }
	Tile_call_print* Tile_call_print::clone() { return new Tile_call_print(); }
	Tile_call_allocate* Tile_call_allocate::clone() { return new Tile_call_allocate(); }
	Tile_call_input* Tile_call_input::clone() { return new Tile_call_input(); }
	Tile_call_tensor_error* Tile_call_tensor_error::clone() { return new Tile_call_tensor_error(); }
	Tile_call_assign* Tile_call_assign::clone() { return new Tile_call_assign(); }
	Tile_call_allocate_assign* Tile_call_allocate_assign::clone() { return new Tile_call_allocate_assign(); }
	Tile_call_input_assign* Tile_call_input_assign::clone() { return new Tile_call_input_assign(); }

	/*
	 * generate target instructions
	 */

	std::string Tile_return::generateTargetInstructions() {
		std::string targetI;
		targetI += "\t\treturn\n";
		return targetI;
	}

	std::string Tile_return_val::generateTargetInstructions() {
		std::string targetI;
		targetI += "\t\trax <- " + dynamic_cast<Instruction_return_val*>(this->instr)->retValue->toString() + "\n";
		targetI += "\t\treturn\n";
		return targetI;
	}

	std::string Tile_mov::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_mov*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_mov*>(this->instr)->src->toString() + "\n";
		return targetI;
	}

	std::string Tile_plus::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_op*>(this->instr)->left->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " += " + dynamic_cast<Instruction_op*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_minus::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_op*>(this->instr)->left->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " -= " + dynamic_cast<Instruction_op*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_times::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_op*>(this->instr)->left->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " *= " + dynamic_cast<Instruction_op*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_and::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_op*>(this->instr)->left->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " &= " + dynamic_cast<Instruction_op*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_lsh::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_op*>(this->instr)->left->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " <<= " + dynamic_cast<Instruction_op*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_rsh::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_op*>(this->instr)->left->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->instr)->dst->name + " >>= " + dynamic_cast<Instruction_op*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_eq::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->instr)->left->toString();
		targetI += " = " + dynamic_cast<Instruction_cmp*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_le::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->instr)->left->toString();
		targetI += " <= " + dynamic_cast<Instruction_cmp*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_ge::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->instr)->right->toString();
		targetI += " <= " + dynamic_cast<Instruction_cmp*>(this->instr)->left->toString() + "\n";
		return targetI;
	}

	std::string Tile_less::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->instr)->left->toString();
		targetI += " < " + dynamic_cast<Instruction_cmp*>(this->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_greater::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->instr)->right->toString();
		targetI += " < " + dynamic_cast<Instruction_cmp*>(this->instr)->left->toString() + "\n";
		return targetI;
	}

	std::string Tile_load::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_load*>(this->instr)->dst->name + " <- mem ";
		targetI += dynamic_cast<Instruction_load*>(this->instr)->src->name + " 0\n";
		return targetI;
	}

	std::string Tile_store::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "mem " + dynamic_cast<Instruction_store*>(this->instr)->dst->name + " 0 <- ";
		targetI += dynamic_cast<Instruction_store*>(this->instr)->src->toString() + "\n";
		return targetI;
	}

	std::string Tile_label::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_label*>(this->instr)->label->name + "\n";
		return targetI;
	}

	std::string Tile_branch::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "goto " + dynamic_cast<Instruction_branch*>(this->instr)->label->name + "\n";
		return targetI;
	}

	std::string Tile_cond_branch::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "cjump " + dynamic_cast<Instruction_cond_branch*>(this->instr)->cond->name + " = 1 ";
		targetI += dynamic_cast<Instruction_cond_branch*>(this->instr)->label->name + "\n";
		return targetI;
	}

	std::string Tile_call::generateTargetInstructions() {
		std::string targetI = "\t\t";
		std::vector<std::string> regArgs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
		auto instC = dynamic_cast<Instruction_call*>(this->instr);
		std::string returnLabel = ":" + instC->dst->toString().substr(1) + "_ret";
		targetI += "mem rsp -8 <- " + returnLabel + "\n";
		for (int i=0; i<instC->args.size(); i++) {
			if (i<6) {
				targetI += "\t\t" + regArgs[i] + " <- " + instC->args[i]->toString() + "\n";
			} else {
				targetI += "\t\tmem rsp -" + std::to_string(8*(i-4)) + " <- " + instC->args[i]->toString() + "\n";
			}
		}
		targetI += "\t\tcall " + instC->dst->toString() + std::to_string(instC->args.size()) + "\n";
		targetI += "\t\t" + returnLabel + "\n";
		return targetI;
	}

	std::string Tile_call_print::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " +  dynamic_cast<Instruction_call_print*>(this->instr)->arg->toString() + "\n";
		targetI += "\t\tcall print 1\n";
		return targetI;
	}

	std::string Tile_call_allocate::generateTargetInstructions() {
		std::string targetI = "\t\t";
		auto instC = dynamic_cast<Instruction_call_allocate*>(this->instr);
		targetI += "rdi <- " + instC->num->toString() + "\n";
		targetI += "\t\trsi <- " + instC->val->toString() + "\n";
		targetI += "\t\tcall allocate 2\n";
		return targetI;
	}

	std::string Tile_call_input::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "call input 0\n";
		return targetI;
	}

	std::string Tile_call_tensor_error::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "call tensor-error " + dynamic_cast<Instruction_call_tensor_error*>(this->instr)->arg->toString() + "\n";
		return targetI;
	}

	std::string Tile_call_assign::generateTargetInstructions() {
		std::string targetI = "\t\t";
		std::vector<std::string> regArgs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
		auto instC = dynamic_cast<Instruction_call_assign*>(this->instr);
		std::string returnLabel = ":" + instC->src->toString().substr(1) + "_ret";
		targetI += "mem rsp -8 <- " + returnLabel + "\n";
		for (int i=0; i<instC->args.size(); i++) {
			if (i<6) {
				targetI += "\t\t" + regArgs[i] + " <- " + instC->args[i]->toString() + "\n";
			} else {
				targetI += "\t\tmem rsp -" + std::to_string(8*(i-4)) + " <- " + instC->args[i]->toString() + "\n";
			}
		}
		targetI += "\t\tcall " + instC->dst->toString() + std::to_string(instC->args.size()) + "\n";
		targetI += "\t\t" + returnLabel + "\n";
		targetI += "\t\t" + instC->dst->name + " <- rax\n";
		return targetI;
	}

	std::string Tile_call_allocate_assign::generateTargetInstructions() {
		std::string targetI = "\t\t";
		auto instC = dynamic_cast<Instruction_call_allocate_assign*>(this->instr);
		targetI += "rdi <- " + instC->num->toString() + "\n";
		targetI += "\t\trsi <- " + instC->val->toString() + "\n";
		targetI += "\t\tcall allocate 2\n";
		targetI += "\t\t" + instC->dst->name + " <- rax\n";
		return targetI;
	}

	std::string Tile_call_input_assign::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "call input 0\n";
		targetI += "\t\t" + dynamic_cast<Instruction_call_input_assign*>(this->instr)->dst->name + " <- rax\n";
		return targetI;
	}


}