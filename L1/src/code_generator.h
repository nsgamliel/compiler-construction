#pragma once

#include <fstream>
#include <string>

#include <L1.h>

namespace L1::x64{

	void generate_code(const Program& p);
  
	// TODO
  std::string conv_label(Item* item);
  std::string conv_operand(Item* item);
  std::string conv_to_8_bit(Item* item);

	class Code_Generator : public L1::Visitor {
		public:
			void open_file(std::string file_name);
			void close_file();
			void convert_L1_to_x64(const Program& p);

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

		private:
			std::ofstream output_file;
	};
	
}
