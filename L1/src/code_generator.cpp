#include <string>
#include <iostream>

#include <code_generator.h>

//using namespace std;


namespace L1::x64{

	bool printGActions = true;

	void generate_code(const Program& p) {
		auto cg = new Code_Generator();

		cg->open_file("prog.S");
		cg->convert_L1_to_x64(p);
		cg->close_file();

		return;
	}

	std::string conv_label(const std::string& str) {
    if (printGActions) std::cout << "converting label " << str << std::endl;
    return "_" + str.substr(1);
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
			return "$" + L1::x64::conv_label(item_c->name);
		} if (dynamic_cast<L1::Memory*> (item)) {
			auto item_c = dynamic_cast<L1::Memory*> (item);
			return std::to_string(item_c->offset->value) + "(%" + item_c->reg->name;
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
		output_file << "\n\tcall " << L1::x64::conv_label(p.entryPointLabel) << "\n\n";
		output_file << "\tpopq %r15\n\tpopq %r14\n\tpopq %r13\n\tpopq %r12\n\tpopq %rbp\n\tpopq %rbx\n\tretq\n\n";

		for (auto f : p.functions) {
			if (f->locals > 0) output_file << "\tsubq $" std::to_string(f->locals*8) << ", %rsp\n";
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
		if (i->stack_alloc > 0) output_file << "\taddq $" std::to_string(i->stack_alloc*8) << ", %rsp\n";
		output_file << "\tretq\n";
		return;
	}

	void Code_Generator::visit(Instruction_mov* i) {
		output_file << "\tmovq " << L1::x64::conv_operand(i->src) << ", " << L1::x64::conv_operand(i->dst) << "\n";
		return;
	}

	void Code_Generator::visit(Instruction_label* i) {
		output_file << L1::x64::conv_label(i->label->value) << ":\n";
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
		output_file << "\tlea (" << L1::x64::conv_operand(i->base) << ", " << L1::x64::conv_operand(i->index) << ", " << L1::x64::conv_operand(i->scale) << "), " << L1::x64::conv_operand(i->dst) << "\n";
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



/*





  //bool printGActions = true;

  void generate_code(Program p){
    
    if (printGActions) std::cout << "opening prog.S..." << std::endl;
    std::ofstream outputFile;
    outputFile.open("prog.S");
    if (printGActions) std::cout << "prog.S opened" << std::endl;
   
    
    if (printGActions) std::cout << "constructing base template..." << std::endl;
    outputFile << "    .text\n    .globl go\ngo:\n    pushq %rbx\n    pushq %rbp\n    pushq %r12\n    pushq %r13\n    pushq %r14\n    pushq %r15\n";
    outputFile << "\n    call " << conv_label(p.entryPointLabel) << "\n\n";
    outputFile << "    popq %r15\n    popq %r14\n    popq %r13\n    popq %r12\n    popq %rbp\n    popq %rbx\n    retq\n\n";
    if (printGActions) std::cout << "base template constructed" << std::endl;


    if (printGActions) std::cout << "entering functions..." << std::endl;
    for (auto f : p.functions) {
      if (printGActions) std::cout << "found function " << f->name << std::endl;
      outputFile << conv_label(f->name) << ":\n";
      if (f->locals > 0)
        outputFile << "    subq $" << std::to_string(f->locals*8) << ", %rsp\n";
      if (printGActions) std::cout << "entering instructions..." << std::endl;
      for (auto i : f->instructions) {
        if (printGActions) std::cout << "found ";
        switch (i->op) {
          case ret:
            if (printGActions) std::cout << "return instruction" << std::endl;
            if (f->arguments > 6 || f->locals > 0)
              outputFile << "    addq $" << std::to_string(f->arguments > 6 ? ((f->arguments-6)+f->locals)*8 : f->locals*8) << ", %rsp\n";
            outputFile << "    retq\n"; break;
          case store:
            if (printGActions) std::cout << "(store) ";
          case load:
            if (printGActions) std::cout << "(load) ";
          case mov:
            if (printGActions) std::cout << "move instruction" << std::endl;
            outputFile << "    movq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case label_def:
            if (printGActions) std::cout << "label def instruction" << std::endl;
            outputFile << conv_operand(i->items[0]) << "\n"; break;
          case aop_pe:
            if (printGActions) std::cout << "aop_pe instruction" << std::endl;
            outputFile << "    addq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_me:
            if (printGActions) std::cout << "aop_me instruction" << std::endl;
            outputFile << "    subq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_te:
            if (printGActions) std::cout << "aop_te instruction" << std::endl;
            outputFile << "    imulq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_ae:
            if (printGActions) std::cout << "aop_ae instruction" << std::endl;
            outputFile << "    andq " << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case aop_pp:
            if (printGActions) std::cout << "aop_pp instruction" << std::endl;
            outputFile << "    inc " << conv_operand(i->items[0]) << "\n"; break;
          case aop_mm:
            if (printGActions) std::cout << "aop_mm instruction" << std::endl;
            outputFile << "    dec " << conv_operand(i->items[0]) << "\n"; break;
          case sop_lsh:
            if (printGActions) std::cout << "sop_lsh instruction" << std::endl;
            outputFile << "    salq " << conv_operand(to_8_bit(i->items[0])) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case sop_rsh:
            if (printGActions) std::cout << "sop_rsh instruction" << std::endl;
            outputFile << "    sarq " << conv_operand(to_8_bit(i->items[0])) << ", " << conv_operand(i->items[1]) << "\n"; break;
          case dir_jmp:
            if (printGActions) std::cout << "dir_jmp instruction" << std::endl;
            outputFile << "    jmp " << conv_label(i->items[0]->value) << "\n"; break;
          case cmp_less:
            if (printGActions) std::cout << "cmp_less instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              outputFile << "    movq $" << int(std::stoi(i->items[1]->value) < std::stoi(i->items[0]->value)) << ", " << conv_operand(i->items[2]) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    setg " << conv_operand(to_8_bit(i->items[2])) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    setl " << conv_operand(to_8_bit(i->items[2])) << "\n";
              }
              outputFile << "    movzbq " << conv_operand(to_8_bit(i->items[2])) << ", " << conv_operand(i->items[2]) << "\n";
            } break;
          case cmp_le:
            if (printGActions) std::cout << "cmp_le instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              outputFile << "    movq $" << int(std::stoi(i->items[1]->value) <= std::stoi(i->items[0]->value)) << ", " << conv_operand(i->items[2]) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    setge " << conv_operand(to_8_bit(i->items[2])) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    setle " << conv_operand(to_8_bit(i->items[2])) << "\n";
              }
              outputFile << "    movzbq " << conv_operand(to_8_bit(i->items[2])) << ", " << conv_operand(i->items[2]) << "\n";
            } break;
          case cmp_eq:
            if (printGActions) std::cout << "cmp_eq instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              outputFile << "    movq $" << int(std::stoi(i->items[1]->value) == std::stoi(i->items[0]->value)) << ", " << conv_operand(i->items[2]) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
              }
              outputFile << "    sete " << conv_operand(to_8_bit(i->items[2])) << "\n";
              outputFile << "    movzbq " << conv_operand(to_8_bit(i->items[2])) << ", " << conv_operand(i->items[2]) << "\n";
            } break;
          case cond_less_jmp:
            if (printGActions) std::cout << "cond_less_jmp instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              if (int(std::stoi(i->items[1]->value) < std::stoi(i->items[0]->value)))
                outputFile << "    jmp " << conv_label(i->items[2]->value) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    jg " << conv_label(i->items[2]->value) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    jl " << conv_label(i->items[2]->value) << "\n";
              }
            } break;
          case cond_le_jmp:
            if (printGActions) std::cout << "cond_le_jmp instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              if (int(std::stoi(i->items[1]->value) <= std::stoi(i->items[0]->value)))
                outputFile << "    jmp " << conv_label(i->items[2]->value) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
                outputFile << "    jge " << conv_label(i->items[2]->value) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
                outputFile << "    jle " << conv_label(i->items[2]->value) << "\n";
              }
            } break;
          case cond_eq_jmp:
            if (printGActions) std::cout << "cond_eq_jmp instruction" << std::endl;
            if (i->items[0]->type == 2 && i->items[1]->type == 2) {
              if (int(std::stoi(i->items[1]->value) == std::stoi(i->items[0]->value)))
                outputFile << "    jmp " << conv_label(i->items[2]->value) << "\n";
            } else {
              outputFile << "    cmpq ";
              if (i->items[1]->type == 2) {
                outputFile << conv_operand(i->items[1]) << ", " << conv_operand(i->items[0]) << "\n";
              } else {
                outputFile << conv_operand(i->items[0]) << ", " << conv_operand(i->items[1]) << "\n";
              }
              outputFile << "    je " << conv_label(i->items[2]->value) << "\n";
            } break;
          case at:
            if (printGActions) std::cout << "at instruction" << std::endl;
            outputFile << "    lea (" << conv_operand(i->items[2]) << ", " << conv_operand(i->items[1]) << ", " << i->items[0]->value << "), " << conv_operand(i->items[3]) << "\n"; break;
          case call_local:
            if (printGActions) std::cout << "local call" << std::endl;
            outputFile << "    subq $" << std::to_string((std::stoi(i->items[1]->value) > 6) ? (std::stoi(i->items[1]->value)-5)*8 : 8) << ", %rsp\n";
            outputFile << "    jmp " << (((i->items[0]->type) == 0) ? ("*" + conv_operand(i->items[0])) : conv_label(i->items[0]->value)) << "\n"; break;          
          case call_runtime:
            if (printGActions) std::cout << "call to runtime" << std::endl;
            outputFile << "    call ";
            if (i->items[0]->value.compare("tensor-error") == 0) {
              switch (std::stoi(i->items[1]->value)) {
                case 1: outputFile << "array_tensor_error_null\n"; break;
                case 3: outputFile << "array_error\n"; break;
                case 4: outputFile << "tensor_error\n"; break;
              }
            } else {
              outputFile << (i->items[0]->value) << "\n";
            } break;
          default:
            if (printGActions) std::cout << "unknown instruction" << std::endl;
            outputFile << "    # instr placeholder\n"; break;
        }
      }
    }


   
    if (printGActions) std::cout << "closing prog.S..." << std::endl;
    outputFile.close();
    if (printGActions) std::cout << "prog.S closed" << std::endl;
   
    return;
  }

  std::string conv_operand(const Item* item) {
    if (printGActions) std::cout << "converting operand ";
    std::string s;
    switch (item->type) {
      case 0:
        if (printGActions) std::cout << "register " << item->register_name << std::endl;
        s = "%" + item->register_name; break;
      case 1:
        if (printGActions) std::cout << "mem access " << item->register_name << " " << item->value << std::endl;
        s = item->value + "(%" + item->register_name + ")"; break;
      case 2:
        if (printGActions) std::cout << "constant " << item->value << std::endl;
        s = "$" + item->value; break;
      case 3:
        if (printGActions) std::cout << "label " << item->value << std::endl;
        s = "$" + conv_label(item->value); break;
      case 4:
        if (printGActions) std::cout << "label definition " << item->value << std::endl;
        s = "    " + conv_label(item->value) + ":"; break;
    }
    if (printGActions) std::cout << "done converting operand" << std::endl;
    return s;
  }

  std::string conv_label(const std::string& str) {
    if (printGActions) std::cout << "converting label " << str << std::endl;
    return "_" + str.substr(1);
  }

  Item* to_8_bit(Item* item) {
    if (printGActions) std::cout << "in to_8_bit" << std::endl;
    if (item->type) return item; // ie only convert registers (type 0)
    if (printGActions) std::cout << "need to convert" << std::endl;
    auto new_i = new Item();
    new_i->type = 0;
    switch (item->r) {
      case rax:
        if (printGActions) std::cout << "8-bit rax" << std::endl;
        new_i->register_name = "al"; break;
      case rbx:
        if (printGActions) std::cout << "8-bit rbx" << std::endl;
        new_i->register_name = "bl"; break;
      case rcx:
        if (printGActions) std::cout << "8-bit rcx" << std::endl;
        new_i->register_name = "cl"; break;
      case rdx:
        if (printGActions) std::cout << "8-bit rdx" << std::endl;
        new_i->register_name = "dl"; break;
      case rdi:
        if (printGActions) std::cout << "8-bit rdi" << std::endl;
        new_i->register_name = "dil"; break;
      case rbp:
        if (printGActions) std::cout << "8-bit rbp" << std::endl;
        new_i->register_name = "bpl"; break;
      case rsi:
        if (printGActions) std::cout << "8-bit rsi" << std::endl;
        new_i->register_name = "sil"; break;
      case r8:
        if (printGActions) std::cout << "8-bit r8" << std::endl;
        new_i->register_name = "r8b"; break;
      case r9:
        if (printGActions) std::cout << "8-bit r9" << std::endl;
        new_i->register_name = "r9b"; break;
      case r10:
        if (printGActions) std::cout << "8-bit r10" << std::endl;
        new_i->register_name = "r10b"; break;
      case r11:
        if (printGActions) std::cout << "8-bit r11" << std::endl;
        new_i->register_name = "r11b"; break;
      case r12:
        if (printGActions) std::cout << "8-bit r12" << std::endl;
        new_i->register_name = "r12b"; break;
      case r13:
        if (printGActions) std::cout << "8-bit r13" << std::endl;
        new_i->register_name = "r13b"; break;
      case r14:
        if (printGActions) std::cout << "8-bit r14" << std::endl;
        new_i->register_name = "r14b"; break;
      case r15:
        if (printGActions) std::cout << "8-bit r15" << std::endl;
        new_i->register_name = "r15b"; break;
    }
    if (printGActions) std::cout << "new register name " << new_i->register_name << std::endl;
    return new_i;
  }*/
}
