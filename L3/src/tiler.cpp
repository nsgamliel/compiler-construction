#include <tiler.h>

#include <iostream>
#include <sstream>

namespace L3 {

	bool printT = false;
	// if (printT) std::cout << "" << std::endl;

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

	bool Tile_call_print_assign::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_print_assign*>(i->instr)) return true;
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

	bool Tile_call_tensor_error_assign::match(InstructionNode* i) {
		if (dynamic_cast<Instruction_call_tensor_error_assign*>(i->instr)) return true;
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
	
	void Tile_return::clone(InstructionNode* i) {
		auto newTile = new Tile_return();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_return_val::clone(InstructionNode* i) {
		auto newTile = new Tile_return_val();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_mov::clone(InstructionNode* i) {
		auto newTile = new Tile_mov();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_plus::clone(InstructionNode* i) {
		auto newTile = new Tile_plus();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_minus::clone(InstructionNode* i) {
		auto newTile = new Tile_minus();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_times::clone(InstructionNode* i) {
		auto newTile = new Tile_times();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_and::clone(InstructionNode* i) {
		auto newTile = new Tile_and();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_lsh::clone(InstructionNode* i) {
		auto newTile = new Tile_lsh();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_rsh::clone(InstructionNode* i) {
		auto newTile = new Tile_rsh();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_eq::clone(InstructionNode* i) {
		auto newTile = new Tile_eq();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_le::clone(InstructionNode* i) {
		auto newTile = new Tile_le();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_ge::clone(InstructionNode* i) {
		auto newTile = new Tile_ge();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_less::clone(InstructionNode* i) {
		auto newTile = new Tile_less();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_greater::clone(InstructionNode* i) {
		auto newTile = new Tile_greater();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_load::clone(InstructionNode* i) {
		auto newTile = new Tile_load();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_store::clone(InstructionNode* i) {
		auto newTile = new Tile_store();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_label::clone(InstructionNode* i) {
		auto newTile = new Tile_label();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_branch::clone(InstructionNode* i) {
		auto newTile = new Tile_branch();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_cond_branch::clone(InstructionNode* i) {
		auto newTile = new Tile_cond_branch();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call::clone(InstructionNode* i) {
		auto newTile = new Tile_call();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_print::clone(InstructionNode* i) {
		auto newTile = new Tile_call_print();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_allocate::clone(InstructionNode* i) {
		auto newTile = new Tile_call_allocate();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_input::clone(InstructionNode* i) {
		auto newTile = new Tile_call_input();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_tensor_error::clone(InstructionNode* i) {
		auto newTile = new Tile_call_tensor_error();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_assign::clone(InstructionNode* i) {
		auto newTile = new Tile_call_assign();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_print_assign::clone(InstructionNode* i) {
		auto newTile = new Tile_call_print_assign();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_allocate_assign::clone(InstructionNode* i) {
		auto newTile = new Tile_call_allocate_assign();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_input_assign::clone(InstructionNode* i) {
		auto newTile = new Tile_call_input_assign();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}

	void Tile_call_tensor_error_assign::clone(InstructionNode* i) {
		auto newTile = new Tile_call_tensor_error_assign();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		return;
	}


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
		targetI += "\t\trax <- " + dynamic_cast<Instruction_return_val*>(this->iNode->instr)->retValue->toString() + "\n";
		targetI += "\t\treturn\n";
		return targetI;
	}

	std::string Tile_mov::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_mov*>(this->iNode->instr)->dst->name + " <- " + dynamic_cast<Instruction_mov*>(this->iNode->instr)->src->toString() + "\n";
		return targetI;
	}

	std::string Tile_plus::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " + dynamic_cast<Instruction_op*>(this->iNode->instr)->left->toString() + "\n\t\t";
		targetI += "rdi += " + dynamic_cast<Instruction_op*>(this->iNode->instr)->right->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->iNode->instr)->dst->name + " <- rdi\n";
		return targetI;
	}

	std::string Tile_minus::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " + dynamic_cast<Instruction_op*>(this->iNode->instr)->left->toString() + "\n\t\t";
		targetI += "rdi -= " + dynamic_cast<Instruction_op*>(this->iNode->instr)->right->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->iNode->instr)->dst->name + " <- rdi\n";
		return targetI;
	}

	std::string Tile_times::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " + dynamic_cast<Instruction_op*>(this->iNode->instr)->left->toString() + "\n\t\t";
		targetI += "rdi *= " + dynamic_cast<Instruction_op*>(this->iNode->instr)->right->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->iNode->instr)->dst->name + " <- rdi\n";
		return targetI;
	}

	std::string Tile_and::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " + dynamic_cast<Instruction_op*>(this->iNode->instr)->left->toString() + "\n\t\t";
		targetI += "rdi &= " + dynamic_cast<Instruction_op*>(this->iNode->instr)->right->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->iNode->instr)->dst->name + " <- rdi\n";
		return targetI;
	}

	std::string Tile_lsh::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " + dynamic_cast<Instruction_op*>(this->iNode->instr)->left->toString() + "\n\t\t";
		targetI += "rdi <<= " + dynamic_cast<Instruction_op*>(this->iNode->instr)->right->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->iNode->instr)->dst->name + " <- rdi\n";
		return targetI;
	}

	std::string Tile_rsh::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " + dynamic_cast<Instruction_op*>(this->iNode->instr)->left->toString() + "\n\t\t";
		targetI += "rdi >>= " + dynamic_cast<Instruction_op*>(this->iNode->instr)->right->toString() + "\n\t\t";
		targetI += dynamic_cast<Instruction_op*>(this->iNode->instr)->dst->name + " <- rdi\n";
		return targetI;
	}

	std::string Tile_eq::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->iNode->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->left->toString();
		targetI += " = " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_le::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->iNode->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->left->toString();
		targetI += " <= " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_ge::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->iNode->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->right->toString();
		targetI += " <= " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->left->toString() + "\n";
		return targetI;
	}

	std::string Tile_less::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->iNode->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->left->toString();
		targetI += " < " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->right->toString() + "\n";
		return targetI;
	}

	std::string Tile_greater::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_cmp*>(this->iNode->instr)->dst->name + " <- " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->right->toString();
		targetI += " < " + dynamic_cast<Instruction_cmp*>(this->iNode->instr)->left->toString() + "\n";
		return targetI;
	}

	std::string Tile_load::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_load*>(this->iNode->instr)->dst->name + " <- mem ";
		targetI += dynamic_cast<Instruction_load*>(this->iNode->instr)->src->name + " 0\n";
		return targetI;
	}

	std::string Tile_store::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "mem " + dynamic_cast<Instruction_store*>(this->iNode->instr)->dst->name + " 0 <- ";
		targetI += dynamic_cast<Instruction_store*>(this->iNode->instr)->src->toString() + "\n";
		return targetI;
	}

	std::string Tile_label::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += dynamic_cast<Instruction_label*>(this->iNode->instr)->label->name + "\n";
		return targetI;
	}

	std::string Tile_branch::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "goto " + dynamic_cast<Instruction_branch*>(this->iNode->instr)->label->name + "\n";
		return targetI;
	}

	std::string Tile_cond_branch::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "cjump " + dynamic_cast<Instruction_cond_branch*>(this->iNode->instr)->cond->name + " = 1 ";
		targetI += dynamic_cast<Instruction_cond_branch*>(this->iNode->instr)->label->name + "\n";
		return targetI;
	}

	std::string Tile_call::generateTargetInstructions() {
		std::ostringstream address;
		address << (void const *)this;
		std::string name = address.str();
		std::string targetI = "\t\t";
		std::vector<std::string> regArgs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
		auto instC = dynamic_cast<Instruction_call*>(this->iNode->instr);
		std::string returnLabel = ":" + instC->dst->toString().substr(1) + "_ret_" + name;
		targetI += "mem rsp -8 <- " + returnLabel + "\n";
		for (int i=0; i<instC->args.size(); i++) {
			if (i<6) {
				targetI += "\t\t" + regArgs[i] + " <- " + instC->args[i]->toString() + "\n";
			} else {
				targetI += "\t\tmem rsp -" + std::to_string(8*(i-4)) + " <- " + instC->args[i]->toString() + "\n";
			}
		}
		targetI += "\t\tcall " + instC->dst->toString() + " " + std::to_string(instC->args.size()) + "\n";
		targetI += "\t\t" + returnLabel + "\n";
		return targetI;
	}

	std::string Tile_call_print::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "rdi <- " +  dynamic_cast<Instruction_call_print*>(this->iNode->instr)->arg->toString() + "\n";
		targetI += "\t\tcall print 1\n";
		return targetI;
	}

	std::string Tile_call_allocate::generateTargetInstructions() {
		std::string targetI = "\t\t";
		auto instC = dynamic_cast<Instruction_call_allocate*>(this->iNode->instr);
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
		std::string targetI;
		std::vector<std::string> regArgs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
		auto instC = dynamic_cast<Instruction_call_tensor_error*>(this->iNode->instr);
		for (int i=0; i<instC->args.size(); i++) {
			targetI += "\t\t" + regArgs[i] + " <- " + instC->args[i]->toString() + "\n";
		}
		targetI += "\t\tcall tensor-error " + std::to_string(instC->args.size()) + "\n";
		return targetI;
	}

	std::string Tile_call_assign::generateTargetInstructions() {
		std::ostringstream address;
		address << (void const *)this;
		std::string name = address.str();
		std::string targetI = "\t\t";
		std::vector<std::string> regArgs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
		auto instC = dynamic_cast<Instruction_call_assign*>(this->iNode->instr);
		std::string returnLabel = ":" + instC->src->toString().substr(1) + "_ret_" + name;
		targetI += "mem rsp -8 <- " + returnLabel + "\n";
		for (int i=0; i<instC->args.size(); i++) {
			if (i<6) {
				targetI += "\t\t" + regArgs[i] + " <- " + instC->args[i]->toString() + "\n";
			} else {
				targetI += "\t\tmem rsp -" + std::to_string(8*(i-4)) + " <- " + instC->args[i]->toString() + "\n";
			}
		}
		targetI += "\t\tcall " + instC->src->toString() + " " + std::to_string(instC->args.size()) + "\n";
		targetI += "\t\t" + returnLabel + "\n";
		targetI += "\t\t" + instC->dst->name + " <- rax\n";
		return targetI;
	}

	std::string Tile_call_print_assign::generateTargetInstructions() {
		std::string targetI = "\t\t";
		auto instC = dynamic_cast<Instruction_call_print_assign*>(this->iNode->instr);
		targetI += "rdi <- " +  instC->arg->toString() + "\n";
		targetI += "\t\tcall print 1\n";
		targetI += "\t\t" + instC->dst->name + " <- rax\n";
		return targetI;
	}

	std::string Tile_call_allocate_assign::generateTargetInstructions() {
		std::string targetI = "\t\t";
		auto instC = dynamic_cast<Instruction_call_allocate_assign*>(this->iNode->instr);
		targetI += "rdi <- " + instC->num->toString() + "\n";
		targetI += "\t\trsi <- " + instC->val->toString() + "\n";
		targetI += "\t\tcall allocate 2\n";
		targetI += "\t\t" + instC->dst->name + " <- rax\n";
		return targetI;
	}

	std::string Tile_call_input_assign::generateTargetInstructions() {
		std::string targetI = "\t\t";
		targetI += "call input 0\n";
		targetI += "\t\t" + dynamic_cast<Instruction_call_input_assign*>(this->iNode->instr)->dst->name + " <- rax\n";
		return targetI;
	}

	std::string Tile_call_tensor_error_assign::generateTargetInstructions() {
		std::string targetI;
		std::vector<std::string> regArgs = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
		auto instC = dynamic_cast<Instruction_call_tensor_error_assign*>(this->iNode->instr);
		for (int i=0; i<instC->args.size(); i++) {
			targetI += "\t\t" + regArgs[i] + " <- " + instC->args[i]->toString() + "\n";
		}
		targetI += "\t\tcall tensor-error " + std::to_string(instC->args.size()) + "\n";
		targetI += "\t\t" + instC->dst->name + " <- rax\n";
		return targetI;
	}

	/*
	 * special tiles
	 */

	bool Tile_mov_special::match(InstructionNode* i) {
		if (i->leaves.size() == 1 && !(i->leaves[0]->isLeaf) && dynamic_cast<Instruction_mov*>(i->instr) && dynamic_cast<Instruction_mov*>(i->leaves[0]->instr)) return true;
		return false;
	}

	bool Tile_at_special::match(InstructionNode* i) {
		InstructionNode* mult = nullptr;
		InstructionNode* base = nullptr;
		if (printT) std::cout << "in at special" << std::endl;
		if (dynamic_cast<Instruction_plus*>(i->instr)) {
			if (printT) std::cout << "is plus" << std::endl;
			for (auto leaf : i->leaves) {
				if (!(leaf->isLeaf)) {
					if (printT) std::cout << "non leaf found" << std::endl;
					auto instC = dynamic_cast<Instruction_times*>(leaf->instr);
					if (instC) {
						if (printT) std::cout << "leaf is times" << std::endl;
						auto num = dynamic_cast<Number*>(instC->left);
						if (num && (num->value == 1 || num->value == 2 || num->value == 4 || num->value == 8)) {
							if (printT) std::cout << "num found" << std::endl;
							if (dynamic_cast<Variable*>(instC->right)) {
								if (printT) std::cout << "var found conditions met" << std::endl;
								mult = leaf;
							}
						} if (!num) {
							if (printT) std::cout << "trying other param" << std::endl;
							num = dynamic_cast<Number*>(instC->right);
							if (num && (num->value == 1 || num->value == 2 || num->value == 4 || num->value == 8)) {
								if (printT) std::cout << "num found" << std::endl;
								if (dynamic_cast<Variable*>(instC->left)) {
									if (printT) std::cout << "var found conditions met" << std::endl;
									mult = leaf;
								}
							}
						}
					}
				} else if (leaf->isLeaf && dynamic_cast<Variable*>(leaf->head)) {
						if (printT) std::cout << "is leaf and var condition met" << std::endl;
						base = leaf;
				}				
			}
			if (printT) std::cout << "attempting tru" << std::endl;
			if (mult && base) return true;
			if (printT) std::cout << "not true" << std::endl;
		}
		if (printT) std::cout << "returning false" << std::endl;
		return false;
	}

	bool Tile_pe_special::match(InstructionNode* i) {
		if (printT) std::cout << "in pe match" << std::endl;
		bool match = false;
		bool source = false;
		auto instC = dynamic_cast<Instruction_plus*>(i->instr);
		if (instC) {
			if (printT) std::cout << "successful dynamic cast" << std::endl;
			for (auto leaf : i->leaves) {
				if (!(leaf->isLeaf)) {
					if (printT) std::cout << "not leaf, attempting move" << std::endl;
					if (dynamic_cast<Instruction_mov*>(leaf->instr)) {
						if (printT) std::cout << "is mov, pulling source" << std::endl;
						if (dynamic_cast<Variable*>(leaf->leaves[0]->head) && dynamic_cast<Variable*>(leaf->leaves[0]->head)->name.compare(instC->dst->name) == 0) {
							if (printT) std::cout << "found match" << std::endl;
							match = true;
						} else {
							if (printT) std::cout << "no match, setting source" << std::endl;
							source = true;
						}
					} else {
						if (printT) std::cout << "not mov, returning false" << std::endl;
						return false;
					}
				} else if ((leaf->head->toString()).compare(instC->dst->name) == 0) {
					if (printT) std::cout << "found match" << std::endl;
					match = true;
				} else {
					if (printT) std::cout << "no match, source found" << std::endl;
					source = true;
				}
			}
			if (printT) std::cout << "checking true" << std::endl;
			if (match && source) return true;
		}
		if (printT) std::cout << "returning false" << std::endl;
		return false;
	}

	bool Tile_me_special::match(InstructionNode* i) {
		if (printT) std::cout << "in me match" << std::endl;
		bool match = false;
		bool source = false;
		auto instC = dynamic_cast<Instruction_minus*>(i->instr);
		if (instC) {
			if (printT) std::cout << "successful dynamic cast" << std::endl;
			if (dynamic_cast<Instruction_mov*>(i->leaves[0]->instr) && (i->leaves[0]->leaves[0]->head->toString()).compare(i->head->toString()) == 0) {
				if (printT) std::cout << "match found in merged mov" << std::endl;
				match = true;
			} else if ((i->leaves[0]->head->toString()).compare(i->head->toString()) == 0) {
				if (printT) std::cout << "match found" << std::endl;
				match = true;
			} else {
				if (printT) std::cout << "no match on left, returning false" << std::endl;
				return false;
			}
			if (i->leaves[1]->isLeaf || dynamic_cast<Instruction_mov*>(i->leaves[1]->instr)) {
				if (printT) std::cout << "adequate source found" << std::endl;
				source = true;
			} else {
				if (printT) std::cout << "invalid source, returning false" << std::endl;
				return false;
			}
			if (printT) std::cout << "checking true" << std::endl;
			if (match && source) return true;
		}
		if (printT) std::cout << "returning false" << std::endl;
		return false;
	}

	bool Tile_te_special::match(InstructionNode* i) {
		if (printT) std::cout << "in te match" << std::endl;
		bool match = false;
		bool source = false;
		auto instC = dynamic_cast<Instruction_times*>(i->instr);
		if (instC) {
			if (printT) std::cout << "successful dynamic cast" << std::endl;
			for (auto leaf : i->leaves) {
				if (!(leaf->isLeaf)) {
					if (printT) std::cout << "not leaf, attempting move" << std::endl;
					if (dynamic_cast<Instruction_mov*>(leaf->instr)) {
						if (printT) std::cout << "is mov, pulling source" << std::endl;
						if (dynamic_cast<Variable*>(leaf->leaves[0]->head) && dynamic_cast<Variable*>(leaf->leaves[0]->head)->name.compare(instC->dst->name) == 0) {
							if (printT) std::cout << "found match" << std::endl;
							match = true;
						} else {
							if (printT) std::cout << "no match, setting source" << std::endl;
							source = true;
						}
					} else {
						if (printT) std::cout << "not mov, returning false" << std::endl;
						return false;
					}
				} else if ((leaf->head->toString()).compare(instC->dst->name) == 0) {
					if (printT) std::cout << "found match" << std::endl;
					match = true;
				} else {
					if (printT) std::cout << "no match, source found" << std::endl;
					source = true;
				}
			}
			if (printT) std::cout << "checking true" << std::endl;
			if (match && source) return true;
		}
		if (printT) std::cout << "returning false" << std::endl;
		return false;
	}

	bool Tile_ae_special::match(InstructionNode* i) {
		if (printT) std::cout << "in ae match" << std::endl;
		bool match = false;
		bool source = false;
		auto instC = dynamic_cast<Instruction_and*>(i->instr);
		if (instC) {
			if (printT) std::cout << "successful dynamic cast" << std::endl;
			for (auto leaf : i->leaves) {
				if (!(leaf->isLeaf)) {
					if (printT) std::cout << "not leaf, attempting move" << std::endl;
					if (dynamic_cast<Instruction_mov*>(leaf->instr)) {
						if (printT) std::cout << "is mov, pulling source" << std::endl;
						if (dynamic_cast<Variable*>(leaf->leaves[0]->head) && dynamic_cast<Variable*>(leaf->leaves[0]->head)->name.compare(instC->dst->name) == 0) {
							if (printT) std::cout << "found match" << std::endl;
							match = true;
						} else {
							if (printT) std::cout << "no match, setting source" << std::endl;
							source = true;
						}
					} else {
						if (printT) std::cout << "not mov, returning false" << std::endl;
						return false;
					}
				} else if ((leaf->head->toString()).compare(instC->dst->name) == 0) {
					if (printT) std::cout << "found match" << std::endl;
					match = true;
				} else {
					if (printT) std::cout << "no match, source found" << std::endl;
					source = true;
				}
			}
			if (printT) std::cout << "checking true" << std::endl;
			if (match && source) return true;
		}
		if (printT) std::cout << "returning false" << std::endl;
		return false;
	}

	bool Tile_lsh_special::match(InstructionNode* i) {
		if (printT) std::cout << "in lsh match" << std::endl;
		bool match = false;
		bool source = false;
		auto instC = dynamic_cast<Instruction_lsh*>(i->instr);
		if (instC) {
			if (printT) std::cout << "successful dynamic cast" << std::endl;
			if (dynamic_cast<Instruction_mov*>(i->leaves[0]->instr) && (i->leaves[0]->leaves[0]->head->toString()).compare(i->head->toString()) == 0) {
				if (printT) std::cout << "match found in merged mov" << std::endl;
				match = true;
			} else if ((i->leaves[0]->head->toString()).compare(i->head->toString()) == 0) {
				if (printT) std::cout << "match found" << std::endl;
				match = true;
			} else {
				if (printT) std::cout << "no match on left, returning false" << std::endl;
				return false;
			}
			if (i->leaves[1]->isLeaf || dynamic_cast<Instruction_mov*>(i->leaves[1]->instr)) {
				if (printT) std::cout << "adequate source found" << std::endl;
				source = true;
			} else {
				if (printT) std::cout << "invalid source, returning false" << std::endl;
				return false;
			}
			if (printT) std::cout << "checking true" << std::endl;
			if (match && source) return true;
		}
		if (printT) std::cout << "returning false" << std::endl;
		return false;
	}

	bool Tile_rsh_special::match(InstructionNode* i) {
		if (printT) std::cout << "in rsh match" << std::endl;
		bool match = false;
		bool source = false;
		auto instC = dynamic_cast<Instruction_rsh*>(i->instr);
		if (instC) {
			if (printT) std::cout << "successful dynamic cast" << std::endl;
			if (dynamic_cast<Instruction_mov*>(i->leaves[0]->instr) && (i->leaves[0]->leaves[0]->head->toString()).compare(i->head->toString()) == 0) {
				if (printT) std::cout << "match found in merged mov" << std::endl;
				match = true;
			} else if ((i->leaves[0]->head->toString()).compare(i->head->toString()) == 0) {
				if (printT) std::cout << "match found" << std::endl;
				match = true;
			} else {
				if (printT) std::cout << "no match on left, returning false" << std::endl;
				return false;
			}
			if (i->leaves[1]->isLeaf || dynamic_cast<Instruction_mov*>(i->leaves[1]->instr)) {
				if (printT) std::cout << "adequate source found" << std::endl;
				source = true;
			} else {
				if (printT) std::cout << "invalid source, returning false" << std::endl;
				return false;
			}
			if (printT) std::cout << "checking true" << std::endl;
			if (match && source) return true;
		}
		if (printT) std::cout << "returning false" << std::endl;
		return false;
	}

	void Tile_mov_special::clone(InstructionNode* i) {
		auto newTile = new Tile_mov_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		return;
	}

	void Tile_at_special::clone(InstructionNode* i) {
		if (printT) std::cout << "tile at special clone" << std::endl;
		auto newTile = new Tile_at_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[1]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		i->leaves[1]->isSpecialChild = true;
	}

	void Tile_pe_special::clone(InstructionNode* i) {
		if (printT) std::cout << "tile pe special clone" << std::endl;
		auto newTile = new Tile_pe_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[1]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		i->leaves[1]->isSpecialChild = true;
	}

	void Tile_me_special::clone(InstructionNode* i) {
		if (printT) std::cout << "tile me special clone" << std::endl;
		auto newTile = new Tile_me_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[1]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		i->leaves[1]->isSpecialChild = true;
	}

	void Tile_te_special::clone(InstructionNode* i) {
		if (printT) std::cout << "tile te special clone" << std::endl;
		auto newTile = new Tile_te_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[1]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		i->leaves[1]->isSpecialChild = true;
	}

	void Tile_ae_special::clone(InstructionNode* i) {
		if (printT) std::cout << "tile ae special clone" << std::endl;
		auto newTile = new Tile_ae_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[1]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		i->leaves[1]->isSpecialChild = true;
	}

	void Tile_lsh_special::clone(InstructionNode* i) {
		if (printT) std::cout << "tile lsh special clone" << std::endl;
		auto newTile = new Tile_lsh_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[1]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		i->leaves[1]->isSpecialChild = true;
	}

	void Tile_rsh_special::clone(InstructionNode* i) {
		if (printT) std::cout << "tile rsh special clone" << std::endl;
		auto newTile = new Tile_rsh_special();
		newTile->iNode = i;
		i->tile = newTile;
		i->isTiled = true;
		i->leaves[0]->isTiled = true;
		i->leaves[1]->isTiled = true;
		i->leaves[0]->isSpecialChild = true;
		i->leaves[1]->isSpecialChild = true;
	}

	std::string Tile_mov_special::generateTargetInstructions() {
		if (printT) std::cout << "generate special move target instructions" << std::endl;
		std::string targetI;
		auto instCD = dynamic_cast<Instruction_mov*>(this->iNode->instr);
		auto instCS = dynamic_cast<Instruction_mov*>(this->iNode->leaves[0]->instr);
		targetI += "\t\t" + instCD->dst->name + " <- " + instCS->src->toString() + "\n";
		return targetI;
	}

	std::string Tile_at_special::generateTargetInstructions() {
		if (printT) std::cout << "generating at target instructions" << std::endl;
		std::string targetI = "\t\t" + dynamic_cast<Variable*>(this->iNode->head)->name + " @ ";
		if (printT) std::cout << "entering for loop" << std::endl;
		for (auto leaf : this->iNode->leaves) {
			if (leaf->isLeaf) {
				targetI += dynamic_cast<Variable*>(leaf->head)->name + " ";
			}
		}
		if (printT) std::cout << "entering second for loop" << std::endl;
		for (auto leaf : this->iNode->leaves) {
			if (!(leaf->isLeaf)) {
				auto instC = dynamic_cast<Instruction_times*>(leaf->instr);
				if (dynamic_cast<Variable*>(instC->left)) {
					targetI += dynamic_cast<Variable*>(instC->left)->name + " " + std::to_string(dynamic_cast<Number*>(instC->right)->value) + "\n";
				} else {
					targetI += dynamic_cast<Variable*>(instC->right)->name + " " + std::to_string(dynamic_cast<Number*>(instC->left)->value) + "\n";
				}
			}
		}
		if (printT) std::cout << "done generating" << std::endl;
		return targetI;
	}

	std::string Tile_pe_special::generateTargetInstructions() {
		if (printT) std::cout << "generating pe target instructions" << std::endl;
		std::string targetI = "\t\t" + dynamic_cast<Variable*>(this->iNode->head)->name + " += ";
		if (printT) std::cout << "finding variables" << std::endl;
		for (auto leaf : this->iNode->leaves) {
			if (!(leaf->isLeaf)) {
				// found a mov, treat appropriately
				if ((leaf->leaves[0]->head->toString()).compare(this->iNode->head->toString()) != 0) {
					if (printT) std::cout << "found source in merged mov" << std::endl;
					targetI += leaf->leaves[0]->head->toString() + "\n";
				}
			} else if ((leaf->head->toString()).compare(this->iNode->head->toString()) != 0) {
				if (printT) std::cout << "found base source" << std::endl;
				targetI += leaf->head->toString() + "\n";
			}
		}
		if (printT) std::cout << "done generating" << std::endl;
		return targetI;
	}

	std::string Tile_me_special::generateTargetInstructions() {
		if (printT) std::cout << "generating me target instructions" << std::endl;
		std::string targetI = "\t\t" + dynamic_cast<Variable*>(this->iNode->head)->name + " -= ";
		if (printT) std::cout << "finding variables" << std::endl;
		if ((this->iNode->leaves[1]->isLeaf)) {
			targetI += this->iNode->leaves[1]->head->toString() + "\n";
		} else {
			targetI += this->iNode->leaves[1]->leaves[0]->head->toString() + "\n";
		}
		if (printT) std::cout << "done generating" << std::endl;
		return targetI;
	}

	std::string Tile_te_special::generateTargetInstructions() {
		if (printT) std::cout << "generating te target instructions" << std::endl;
		std::string targetI = "\t\t" + dynamic_cast<Variable*>(this->iNode->head)->name + " *= ";
		if (printT) std::cout << "finding variables" << std::endl;
		for (auto leaf : this->iNode->leaves) {
			if (!(leaf->isLeaf)) {
				// found a mov, treat appropriately
				if ((leaf->leaves[0]->head->toString()).compare(this->iNode->head->toString()) != 0) {
					if (printT) std::cout << "found source in merged mov" << std::endl;
					targetI += leaf->leaves[0]->head->toString() + "\n";
				}
			} else if ((leaf->head->toString()).compare(this->iNode->head->toString()) != 0) {
				if (printT) std::cout << "found base source" << std::endl;
				targetI += leaf->head->toString() + "\n";
			}
		}
		if (printT) std::cout << "done generating" << std::endl;
		return targetI;
	}

	std::string Tile_ae_special::generateTargetInstructions() {
		if (printT) std::cout << "generating ae target instructions" << std::endl;
		std::string targetI = "\t\t" + dynamic_cast<Variable*>(this->iNode->head)->name + " &= ";
		if (printT) std::cout << "finding variables" << std::endl;
		for (auto leaf : this->iNode->leaves) {
			if (!(leaf->isLeaf)) {
				// found a mov, treat appropriately
				if ((leaf->leaves[0]->head->toString()).compare(this->iNode->head->toString()) != 0) {
					if (printT) std::cout << "found source in merged mov" << std::endl;
					targetI += leaf->leaves[0]->head->toString() + "\n";
				}
			} else if ((leaf->head->toString()).compare(this->iNode->head->toString()) != 0) {
				if (printT) std::cout << "found base source" << std::endl;
				targetI += leaf->head->toString() + "\n";
			}
		}
		if (printT) std::cout << "done generating" << std::endl;
		return targetI;
	}

	std::string Tile_lsh_special::generateTargetInstructions() {
		if (printT) std::cout << "generating lsh target instructions" << std::endl;
		std::string targetI = "\t\t" + dynamic_cast<Variable*>(this->iNode->head)->name + " <<= ";
		if (printT) std::cout << "finding variables" << std::endl;
		if ((this->iNode->leaves[1]->isLeaf)) {
			targetI += this->iNode->leaves[1]->head->toString() + "\n";
		} else {
			targetI += this->iNode->leaves[1]->leaves[0]->head->toString() + "\n";
		}
		if (printT) std::cout << "done generating" << std::endl;
		return targetI;
	}

	std::string Tile_rsh_special::generateTargetInstructions() {
		if (printT) std::cout << "generating rsh target instructions" << std::endl;
		std::string targetI = "\t\t" + dynamic_cast<Variable*>(this->iNode->head)->name + " >>= ";
		if (printT) std::cout << "finding variables" << std::endl;
		if ((this->iNode->leaves[1]->isLeaf)) {
			targetI += this->iNode->leaves[1]->head->toString() + "\n";
		} else {
			targetI += this->iNode->leaves[1]->leaves[0]->head->toString() + "\n";
		}
		if (printT) std::cout << "done generating" << std::endl;
		return targetI;
	}

}