#pragma once

#include <string>

#include <L2.h>

namespace L2 {

	class Spiller : public Visitor {
		public:
			Function* spill(Function* f, Variable* svar, std::string prefix);
			Variable* spill_var;
			std::string spill_prefix;
			int num_replace=0;

			void visit(Instruction_return* i)            override;
			void visit(Instruction_mov* i)               override;
			void visit(Instruction_label* i)             override;
			void visit(Instruction_aop_pe* i)            override;
			void visit(Instruction_aop_me* i)            override;
			void visit(Instruction_aop_te* i)            override;
			void visit(Instruction_aop_ae* i)            override;
			void visit(Instruction_aop_pp* i)            override;
			void visit(Instruction_aop_mm* i)            override;
			void visit(Instruction_sop_lsh* i)           override;
			void visit(Instruction_sop_rsh* i)           override;
			void visit(Instruction_dir_jmp* i)           override;
			void visit(Instruction_cmp_less* i)          override;
			void visit(Instruction_cmp_le* i)            override;
			void visit(Instruction_cmp_eq* i)            override;
			void visit(Instruction_cnd_jmp_less* i)      override;
			void visit(Instruction_cnd_jmp_le* i)        override;
			void visit(Instruction_cnd_jmp_eq* i)        override;
			void visit(Instruction_at* i)                override;
			void visit(Instruction_load* i)              override;
			void visit(Instruction_store* i)             override;
			void visit(Instruction_call* i)              override;
			void visit(Instruction_call_print* i)        override;
			void visit(Instruction_call_input* i)        override;
			void visit(Instruction_call_allocate* i)     override;
			void visit(Instruction_call_tensor_error* i) override;
			void visit(Instruction_load_stack_arg* i)    override;
	};

	void generate_spill_output(Function* f);
	
}