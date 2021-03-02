#pragma once

#include <L3.h>

namespace L3 {

	class GenKillGenerator : public Visitor {
		public:
			void visit(Instruction_return* i) override;
			void visit(Instruction_return_val* i) override;
			void visit(Instruction_mov* i) override;
			void visit(Instruction_plus* i) override;
			void visit(Instruction_minus* i) override;
			void visit(Instruction_times* i) override;
			void visit(Instruction_and* i) override;
			void visit(Instruction_lsh* i) override;
			void visit(Instruction_rsh* i) override;
			void visit(Instruction_eq* i) override;
			void visit(Instruction_le* i) override;
			void visit(Instruction_ge* i) override;
			void visit(Instruction_less* i) override;
			void visit(Instruction_greater* i) override;
			void visit(Instruction_load* i) override;
			void visit(Instruction_store* i) override;
			void visit(Instruction_label* i) override;
			void visit(Instruction_branch* i) override;
			void visit(Instruction_cond_branch* i) override;
			void visit(Instruction_call* i) override;
			void visit(Instruction_call_print* i) override;
			void visit(Instruction_call_allocate* i) override;
			void visit(Instruction_call_input* i) override;
			void visit(Instruction_call_tensor_error* i) override;
			void visit(Instruction_call_assign* i) override;
			void visit(Instruction_call_print_assign* i) override;
			void visit(Instruction_call_allocate_assign* i) override;
			void visit(Instruction_call_input_assign* i) override;
			void visit(Instruction_call_tensor_error_assign* i) override;
	};

}