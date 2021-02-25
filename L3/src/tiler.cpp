#include <tiler.h>

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

	void Tile_return::accept(TileVisitor* tv) { return; }
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
	void Tile_call_input_assign::accept(TileVisitor* tv) { return; }

}