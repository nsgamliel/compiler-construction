#pragma once

#include <fstream>

#include <L1.h>

namespace L1{

  

  //std::string conv_label(const std::string& str);
  //std::string conv_operand(const Item* item);
  //Item* to_8_bit(Item* item);

	// TODO: code generator class
	class Code_Generator : public L1::Visitor {
		public:
			std::ofstream output_file;
			void visit(Instruction_return* i) override;
	};

	// TODO: code generation function
	void convert_L1_to_x64(const Program& p);

}
