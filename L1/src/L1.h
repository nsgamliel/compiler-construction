#pragma once

#include <vector>
#include <string>

namespace L1 {

	// enum Opcode {ret, mov, label_def, aop_pe, aop_me, aop_te, aop_ae, aop_pp, aop_mm, sop_lsh, sop_rsh, dir_jmp, cmp_less, cmp_le, cmp_eq, cond_less_jmp, cond_le_jmp, cond_eq_jmp, at, load, store, call_local, call_runtime};

	class Visitor;

	// TODO: items
	class Item {};

	class Variable : public Item {
		public:
			Variable(std::string name);
			
		private:
			std::string name;
	};

	class Register : public Item {
		public:
			Register(std::string name);
			
		private:
			std::string name;
	};

	// TODO: instructions
	class Instruction {
		public:
			virtual void accept(Visitor* v) = 0;
	};

	class Instruction_return : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

  class Function{
		public:
	    std::string name;
  	  int64_t arguments;
   		int64_t locals;
    	std::vector<Instruction *> instructions;
  };

  class Program{
		public:
	    std::string entryPointLabel;
  	  std::vector<Function *> functions;
  };

	// TODO: visitor
	class Visitor {
		public:
			virtual void visit(Instruction_return* i) = 0;
	};

}
