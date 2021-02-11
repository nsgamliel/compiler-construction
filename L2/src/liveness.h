#pragma once

#include <map>

#include <L2.h>

namespace L2 {

	void generate_liveness_output(L2::Function* f);

	class GenKill_Generator : public Visitor {
		public:
			L2::Variable* conv_item(Item* item);
			std::vector<L2::Variable*> callee_save = {new Variable("r12"), new Variable("r13"), new Variable("r14"), new Variable("r15"), new Variable("rbp"), new Variable("rbx")};
			std::vector<L2::Variable*> caller_save = {new Variable("rdi"), new Variable("rsi"), new Variable("rdx"), new Variable("rcx"), new Variable("r8"), new Variable("r9"), new Variable("r10"), new Variable("r11"), new Variable("rax")};

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

}