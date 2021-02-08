#include <string>
#include <iostream>

#include <code_generator.h>

//using namespace std;


namespace L1::x64{

	bool printGActions = false;

	void generate_code(const Program& p) {
		auto cg = new Code_Generator();

		cg->open_file("prog.S");
		cg->convert_L1_to_x64(p);
		cg->close_file();

		return;
	}

	std::string conv_label(Item* item) {
		auto item_c = dynamic_cast<Label*> (item);
		if (printGActions) std::cout << "converting label" << std::endl;
		if (item_c) {
    	return "_" + item_c->name.substr(1);
		} else {
			std::cerr << "not a label type" << std::endl;
			return "";
		}
  }

	std::string conv_operand(Item* item) {
		if (dynamic_cast<L1::Register*> (item)) {
			auto item_c = dynamic_cast<L1::Register*> (item);
			return "%" + item_c->name;
		} if (dynamic_cast<L1::Number*> (item)){
			auto item_c = dynamic_cast<L1::Number*> (item);
			return "$" + std::to_string(item_c->value);
		} if (dynamic_cast<L1::Label*> (item)) {
			auto item_c = dynamic_cast<L1::Label*> (item);
			return "$" + L1::x64::conv_label(item_c);
		} if (dynamic_cast<L1::Memory*> (item)) {
			auto item_c = dynamic_cast<L1::Memory*> (item);
			return std::to_string(item_c->offset->value) + "(%" + item_c->reg->name + ")";
		} else
			return "<<error>>";
	}

	std::string conv_to_8_bit(Item* item) {
		if (!dynamic_cast<L1::Register*> (item))
			return conv_operand(item);
		auto item_c = dynamic_cast<Register*> (item);
		switch (item_c->rid) {
			case r10:
				return "%r10b"; break;
			case r11:
				return "%r11b"; break;
			case r12:
				return "%r12b"; break;
			case r13:
				return "%r13b"; break;
			case r14:
				return "%r14b"; break;
			case r15:
				return "%r15b"; break;
			case r8:
				return "%r8b"; break;
			case r9:
				return "%r9b"; break;
			case rax:
				return "%al"; break;
			case rbp:
				return "%bpl"; break;
			case rbx:
				return "%bl"; break;
			case rcx:
				return "%cl"; break;
			case rdi:
				return "%dil"; break;
			case rdx:
				return "%dl"; break;
			case rsi:
				return "%sil"; break;
			default:
				return "registernotfound"; break;
		}
	}

	void Code_Generator::open_file(std::string file_name) {
		output_file.open(file_name);
		return;
	}

	void Code_Generator::close_file() {
		output_file.close();
		return;
	}

	void Code_Generator::convert_L1_to_x64(const Program& p) {
		output_file << "\t.text\n\t.globl go\ngo:\n\tpushq %rbx\n\tpushq %rbp\n\tpushq %r12\n\tpushq %r13\n\tpushq %r14\n\tpushq %r15\n";
		output_file << "\n\tcall _" << p.entryPointLabel.substr(1) << "\n\n";
		output_file << "\tpopq %r15\n\tpopq %r14\n\tpopq %r13\n\tpopq %r12\n\tpopq %rbp\n\tpopq %rbx\n\tretq\n\n";

		for (auto f : p.functions) {
			output_file << "_" << f->name.substr(1) << ":\n";
			if (f->locals > 0) output_file << "\tsubq $" << std::to_string(f->locals*8) << ", %rsp\n";
			for (auto i : f->instructions) {
				i->accept(this);
			}
		}

		return;
	}

	/*
	 * code generator visit overrides
	 */

	void Code_Generator::visit(Instruction_return* i) {
		if (i->stack_alloc > 0) output_file << "\taddq $" << std::to_string(i->stack_alloc*8) << ", %rsp\n";
		output_file << "\tretq\n";
		return;
	}

	void Code_Generator::visit(Instruction_mov* i) {
		output_file << "\tmovq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_label* i) {
		output_file << L1::x64::conv_label(i->label) << ":\n";
		return;
	}

	void Code_Generator::visit(Instruction_aop_pe* i) {
		output_file << "\taddq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_aop_me* i) {
		output_file << "\tsubq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_aop_te* i) {
		output_file << "\timulq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_aop_ae* i) {
		output_file << "\tandq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_aop_pp* i) {
		output_file << "\tinc " << L1::x64::conv_operand(i->src) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_aop_mm* i) {
		output_file << "\tdec " << L1::x64::conv_operand(i->src) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_sop_lsh* i) {
		output_file << "\tsalq " << L1::x64::conv_to_8_bit(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_sop_rsh* i) {
		output_file << "\tsarq " << L1::x64::conv_to_8_bit(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_dir_jmp* i) {
		output_file << "\tjmp " << L1::x64::conv_label(i->label) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_cmp_less* i) {
		if (dynamic_cast<Number*> (i->left) && dynamic_cast<Number*> (i->right)) {
			output_file << "\tmovq $" << int((dynamic_cast<Number*> (i->left))->value < (dynamic_cast<Number*> (i->right))->value) << ", " << L1::x64::conv_operand(i->dst) << "\n";
			return;
		} 
		if (dynamic_cast<Number*> (i->left)) {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->left) << ", " << L1::x64::conv_operand(i->right) << "\n";
			output_file << "\tsetg ";
		} else {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->right) << ", " << L1::x64::conv_operand(i->left) << "\n";
			output_file << "\tsetl ";
		}
		output_file << L1::x64::conv_to_8_bit(i->dst) << "\n";
		output_file << "\tmovzbq " << L1::x64::conv_to_8_bit(i->dst) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_cmp_le* i) {
		if (dynamic_cast<Number*> (i->left) && dynamic_cast<Number*> (i->right)) {
			output_file << "\tmovq $" << int((dynamic_cast<Number*> (i->left))->value <= (dynamic_cast<Number*> (i->right))->value) << ", " << L1::x64::conv_operand(i->dst) << "\n";
			return;
		}
		if (dynamic_cast<Number*> (i->left)) {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->left) << ", " << L1::x64::conv_operand(i->right) << "\n";
			output_file << "\tsetge ";
		} else {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->right) << ", " << L1::x64::conv_operand(i->left) << "\n";
			output_file << "\tsetle ";
		}
		output_file << L1::x64::conv_to_8_bit(i->dst) << "\n";
		output_file << "\tmovzbq " << L1::x64::conv_to_8_bit(i->dst) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_cmp_eq* i) {
		if (dynamic_cast<Number*> (i->left) && dynamic_cast<Number*> (i->right)) {
			output_file << "\tmovq $" << int((dynamic_cast<Number*> (i->left))->value == (dynamic_cast<Number*> (i->right))->value) << ", " << L1::x64::conv_operand(i->dst) << "\n";
			return;
		}
		if (dynamic_cast<Number*> (i->left)) {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->left) << ", " << L1::x64::conv_operand(i->right) << "\n";
		} else {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->right) << ", " << L1::x64::conv_operand(i->left) << "\n";
		}
		output_file << "\tsete " << L1::x64::conv_to_8_bit(i->dst) << "\n";
		output_file << "\tmovzbq " << L1::x64::conv_to_8_bit(i->dst) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_cnd_jmp_less* i) {
		if (dynamic_cast<Number*> (i->left) && dynamic_cast<Number*> (i->right)) {
			if ((dynamic_cast<Number*> (i->left))->value < (dynamic_cast<Number*> (i->right))->value)
				output_file << "\tjmp " << L1::x64::conv_label(i->dst) << "\n";
			return;
		} 
		if (dynamic_cast<Number*> (i->left)) {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->left) << ", " << L1::x64::conv_operand(i->right) << "\n";
			output_file << "\tjg " << L1::x64::conv_label(i->dst) << "\n";
		} else {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->right) << ", " << L1::x64::conv_operand(i->left) << "\n";
			output_file << "\tjl " << L1::x64::conv_label(i->dst) << "\n";
		}
		return;
	}

	void Code_Generator::visit(Instruction_cnd_jmp_le* i) {
		if (dynamic_cast<Number*> (i->left) && dynamic_cast<Number*> (i->right)) {
			if ((dynamic_cast<Number*> (i->left))->value <= (dynamic_cast<Number*> (i->right))->value)
				output_file << "\tjmp " << L1::x64::conv_label(i->dst) << "\n";
			return;
		} 
		if (dynamic_cast<Number*> (i->left)) {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->left) << ", " << L1::x64::conv_operand(i->right) << "\n";
			output_file << "\tjge " << L1::x64::conv_label(i->dst) << "\n";
		} else {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->right) << ", " << L1::x64::conv_operand(i->left) << "\n";
			output_file << "\tjle " << L1::x64::conv_label(i->dst) << "\n";
		}
		return;
	}

	void Code_Generator::visit(Instruction_cnd_jmp_eq* i) {
		if (dynamic_cast<Number*> (i->left) && dynamic_cast<Number*> (i->right)) {
			if ((dynamic_cast<Number*> (i->left))->value == (dynamic_cast<Number*> (i->right))->value)
				output_file << "\tjmp " << L1::x64::conv_label(i->dst) << "\n";
			return;
		} 
		if (dynamic_cast<Number*> (i->left)) {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->left) << ", " << L1::x64::conv_operand(i->right) << "\n";
		} else {
			output_file << "\tcmpq " << L1::x64::conv_operand(i->right) << ", " << L1::x64::conv_operand(i->left) << "\n";
		}
		output_file << "\tje " << L1::x64::conv_label(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_at* i) {
		output_file << "\tlea (" << L1::x64::conv_operand(i->base) << ", " << L1::x64::conv_operand(i->index) << ", " << L1::x64::conv_operand(i->scale).substr(1) << "), " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_load* i) {
		output_file << "\tmovq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_store* i) {
		output_file << "\tmovq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_call* i) {
		output_file << "\tsubq $" << std::to_string((i->args->value > 6) ? (i->args->value - 5)*8 : 8) << ", %rsp\n";
		if (dynamic_cast<Register*> (i->dst))
			output_file << "\tjmp *" << L1::x64::conv_operand(i->dst) << "\n";
		else
			output_file << "\tjmp " << L1::x64::conv_label(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_call_print* i) {
		output_file << "\tcall print\n";
		return;
	}

	void Code_Generator::visit(Instruction_call_input* i) {
		output_file << "\tcall input\n";
		return;
	}

	void Code_Generator::visit(Instruction_call_allocate* i) {
		output_file << "\tcall allocate\n";
	}

	void Code_Generator::visit(Instruction_call_tensor_error* i) {
		switch (i->args->value) {
			case 1:
				output_file << "\tcall array_tensor_error_null\n"; break;
			case 3:
				output_file << "\tcall array_error\n"; break;
			case 4:
				output_file << "\tcall tensor_error\n"; break;
			default:
				output_file << "\t#tensor_error call not found\n";
		}
		return;
	}
	
}
