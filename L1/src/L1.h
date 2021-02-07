#pragma once

#include <vector>
#include <string>

namespace L1 {

	class Visitor;

	/*
	 * items
	 */

	class Item {};

	class Label : public Item {
		public:
			Label(const std::string& name);
			
		private:
			std::string name;
	};

	class Register : public Item {
		public:
			Register(const std::string& name);
			
		private:
			std::string name;
	};

	class Number : public Item {
		public:
			Number(int64_t num);

		private:
			int64_t value;
	}

	class Memory : public Item {
		public:
			Memory(const Register& reg, )
	}

	/*
	 * instructions
	 */

	class Instruction {
		public:
			virtual void accept(Visitor* v) = 0;
	};

	class Instruction_return : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_mov : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_label : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_aop_pe : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_aop_me : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_aop_te : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_aop_ae : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_aop_pp : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_aop_mm : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_sop_lsh : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_sop_rsh : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_dir_jmp : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_cmp_less : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_cmp_le : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_cmp_eq : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_cnd_jmp_less : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_cnd_jmp_le : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_cnd_jmp_eq : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_at : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_load : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_store : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_call_local : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_call_runtime : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	/*
	 * general
	 */

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

	class Visitor {
		public:
			virtual void visit(Instruction_return* i)       = 0;
			virtual void visit(Instruction_mov* i)          = 0;
			virtual void visit(Instruction_label* i)        = 0;
			virtual void visit(Instruction_aop_pe* i)       = 0;
			virtual void visit(Instruction_aop_me* i)       = 0;
			virtual void visit(Instruction_aop_te* i)       = 0;
			virtual void visit(Instruction_aop_ae* i)       = 0;
			virtual void visit(Instruction_aop_pp* i)       = 0;
			virtual void visit(Instruction_aop_mm* i)       = 0;
			virtual void visit(Instruction_sop_lsh* i)      = 0;
			virtual void visit(Instruction_sop_rsh* i)      = 0;
			virtual void visit(Instruction_dir_jmp* i)      = 0;
			virtual void visit(Instruction_cmp_less* i)     = 0;
			virtual void visit(Instruction_cmp_le* i)       = 0;
			virtual void visit(Instruction_cmp_eq* i)       = 0;
			virtual void visit(Instruction_cnd_jmp_less* i) = 0;
			virtual void visit(Instruction_cnd_jmp_le* i)   = 0;
			virtual void visit(Instruction_cnd_jmp_eq* i)   = 0;
			virtual void visit(Instruction_at* i)           = 0;
			virtual void visit(Instruction_load* i)         = 0;
			virtual void visit(Instruction_store* i)        = 0;
			virtual void visit(Instruction_call_local* i)   = 0;
			virtual void visit(Instruction_call_runtime* i) = 0;
	};

}
