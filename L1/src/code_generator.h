#pragma once

#include <fstream>

#include <L1.h>

namespace L1{

	// TODO: code generation function
	void convert_L1_to_x64(const Program& p);
  

  //std::string conv_label(const std::string& str);
  //std::string conv_operand(const Item* item);
  //Item* to_8_bit(Item* item);

	// TODO: code generator class
	class Code_Generator : public L1::Visitor {
		public:			
			void visit(Instruction_return* i)       override;
			void visit(Instruction_mov* i)          override;
			void visit(Instruction_label* i)        override;
			void visit(Instruction_aop_pe* i)       override;
			void visit(Instruction_aop_me* i)       override;
			void visit(Instruction_aop_te* i)       override;
			void visit(Instruction_aop_ae* i)       override;
			void visit(Instruction_aop_pp* i)       override;
			void visit(Instruction_aop_mm* i)       override;
			void visit(Instruction_sop_lsh* i)      override;
			void visit(Instruction_sop_rsh* i)      override;
			void visit(Instruction_dir_jmp* i)      override;
			void visit(Instruction_cmp_less* i)     override;
			void visit(Instruction_cmp_le* i)       override;
			void visit(Instruction_cmp_eq* i)       override;
			void visit(Instruction_cnd_jmp_less* i) override;
			void visit(Instruction_cnd_jmp_le* i)   override;
			void visit(Instruction_cnd_jmp_eq* i)   override;
			void visit(Instruction_at* i)           override;
			void visit(Instruction_load* i)         override;
			void visit(Instruction_store* i)        override;
			void visit(Instruction_call_local* i)   override;
			void visit(Instruction_call_runtime* i) override;

		private:
			std::ofstream output_file;
	};
	
}
