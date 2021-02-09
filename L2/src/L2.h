#pragma once

#include <vector>
#include <string>
#include <map>

namespace L2 {

	class Visitor;

	/*
	 * items
	 */

	class Item {
		public:
			virtual ~Item(){}; // allows types to be polymorphic for dynamic casting
	};

	class Label : public Item {
		public:
			Label(const std::string& str);
			
		//private:
			std::string name;
	};

	class Variable : public Item {
		public:
			Variable(const std::string& n);
			bool is_in(std::vector<Variable*> vec);

		//private:
			std::string name;
	};

	enum RegisterId {rax, rbx, rcx, rdx, rdi, rsi, rbp, r8, r9, r10, r11, r12, r13, r14, r15, rsp};

	class Register : public Variable {
		public:
			Register(const std::string& n, RegisterId r);
			
		//private:
			RegisterId rid;
	};

	class Number : public Item {
		public:
			Number(int64_t n);

		//private:
			int64_t value;
	};

	class Memory : public Item {
		public:
			Memory(Variable* r, Number* n);

		//private:
			Variable* var;
			Number* offset;
	};

	/*
	 * instructions
	 */

	class Instruction {
		public:
			virtual void accept(Visitor* v) = 0;
			
		//private:
			std::vector<Variable*> gen;
			std::vector<Variable*> kill;
			std::vector<Variable*> in;
			std::vector<Variable*> out;
			std::vector<Instruction*> successors;
	};

	class Instruction_op_one : public Instruction {
		public:
			Instruction_op_one(Item* s);
			virtual void accept(Visitor* v) = 0;
		
		//private:
			Item* src;
	};

	class Instruction_op_two : public Instruction {
		public:
			Instruction_op_two(Item* s, Item* d);
			virtual void accept(Visitor* v) = 0;
		
		//private:
			Item* src;
			Item* dst;
	};

	class Instruction_return : public Instruction {
		public:
			Instruction_return(int64_t n);
			void accept(Visitor* v) override;
		
		//private:
			int64_t stack_alloc;
	};

	class Instruction_mov : public Instruction_op_two {
		public:
			Instruction_mov(Item* s, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_label : public Instruction {
		public:
			Instruction_label(Label* l);
			void accept(Visitor* v) override;

		//private:
			Label* label;
	};

	class Instruction_aop_pe : public Instruction_op_two {
		public:
			Instruction_aop_pe(Item* s, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_aop_me : public Instruction_op_two {
		public:
			Instruction_aop_me(Item* s, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_aop_te : public Instruction_op_two {
		public:
			Instruction_aop_te(Item* s, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_aop_ae : public Instruction_op_two {
		public:
			Instruction_aop_ae(Item* s, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_aop_pp : public Instruction_op_one {
		public:
			Instruction_aop_pp(Item* s);
			void accept(Visitor* v) override;
	};

	class Instruction_aop_mm : public Instruction_op_one {
		public:
			Instruction_aop_mm(Item* s);
			void accept(Visitor* v) override;
	};

	class Instruction_sop_lsh : public Instruction_op_two {
		public:
			Instruction_sop_lsh(Item* s, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_sop_rsh : public Instruction_op_two {
		public:
			Instruction_sop_rsh(Item* s, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_dir_jmp : public Instruction_label {
		public:
			Instruction_dir_jmp(Label* l);
			void accept(Visitor* v) override;
	};

	class Instruction_cmp : public Instruction {
		public:
			Instruction_cmp(Item* l, Item* r, Item* d);
			virtual void accept(Visitor* v) = 0;

		//private:
			Item* left;
			Item* right;
			Item* dst;
	};

	class Instruction_cmp_less : public Instruction_cmp {
		public:
			Instruction_cmp_less(Item* l, Item* r, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_cmp_le : public Instruction_cmp {
		public:
			Instruction_cmp_le(Item* l, Item* r, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_cmp_eq : public Instruction_cmp {
		public:
			Instruction_cmp_eq(Item* l, Item* r, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_cnd_jmp : public Instruction {
		public:
			Instruction_cnd_jmp(Item* l, Item* r, Label* d);
			virtual void accept(Visitor* v) = 0;

		//private:
			Item* left;
			Item* right;
			Label* dst;
	};

	class Instruction_cnd_jmp_less : public Instruction_cnd_jmp {
		public:
			Instruction_cnd_jmp_less(Item* l, Item* r, Label* d);
			void accept(Visitor* v) override;
	};

	class Instruction_cnd_jmp_le : public Instruction_cnd_jmp {
		public:
			Instruction_cnd_jmp_le(Item* l, Item* r, Label* d);
			void accept(Visitor* v) override;
	};

	class Instruction_cnd_jmp_eq : public Instruction_cnd_jmp {
		public:
			Instruction_cnd_jmp_eq(Item* l, Item* r, Label* d);
			void accept(Visitor* v) override;
	};

	class Instruction_at : public Instruction {
		public:
			Instruction_at(Register* b, Register* i, Number* s, Register* d);
			void accept(Visitor* v) override;

		//private:
			// dst = base + index*scale
			Register* base;
			Register* index;
			Number* scale;
			Register* dst;
	};

	class Instruction_load : public Instruction_op_two {
		public:
			Instruction_load(Memory* s, Item* d);
			void accept(Visitor* v) override;
	};

	class Instruction_store : public Instruction_op_two {
		public:
			Instruction_store(Item* s, Memory* d);
			void accept(Visitor* v) override;
	};

	class Instruction_call : public Instruction {
		public:
			Instruction_call(Item* d, Number* a);
			void accept(Visitor* v) override;

		//private:
			Item* dst;
			Number* args;
	};
	
	class Instruction_call_print : public Instruction_call {
		public:
			Instruction_call_print();
			void accept(Visitor* v) override;
	};

	class Instruction_call_input : public Instruction_call {
		public:
			Instruction_call_input();
			void accept(Visitor* v) override;
	};

	class Instruction_call_allocate : public Instruction_call {
		public:
			Instruction_call_allocate();
			void accept(Visitor* v) override;
	};

	class Instruction_call_tensor_error : public Instruction_call {
		public:
			Instruction_call_tensor_error(Number* a);
			void accept(Visitor* v) override;
	};

	class Instruction_load_stack_arg : public Instruction {
		public:
			Instruction_load_stack_arg(Number* s, Item* d);
			void accept(Visitor* v) override;

		//private:
			Number* offset;
			Item* dst;
	};

	/*
	 * general
	 */

  class Function{
		public:
			void generate_liveness();
			// helpers
			void gen_kill();
			void find_successors();
			void in_out();

	    std::string name;
  	  int64_t arguments;
   		int64_t locals = 0;
			int num_replace = 0;
    	std::vector<Instruction *> instructions;
			// additions for liveness etc
			bool isDirty;
			std::map<size_t, std::string> items_l;
  };

  class Program{
		public:
	    std::string entryPointLabel;
  	  std::vector<Function *> functions;
  };

	class Visitor {
		public:
			virtual void visit(Instruction_return* i)            = 0;
			virtual void visit(Instruction_mov* i)               = 0;
			virtual void visit(Instruction_label* i)             = 0;
			virtual void visit(Instruction_aop_pe* i)            = 0;
			virtual void visit(Instruction_aop_me* i)            = 0;
			virtual void visit(Instruction_aop_te* i)            = 0;
			virtual void visit(Instruction_aop_ae* i)            = 0;
			virtual void visit(Instruction_aop_pp* i)            = 0;
			virtual void visit(Instruction_aop_mm* i)            = 0;
			virtual void visit(Instruction_sop_lsh* i)           = 0;
			virtual void visit(Instruction_sop_rsh* i)           = 0;
			virtual void visit(Instruction_dir_jmp* i)           = 0;
			virtual void visit(Instruction_cmp_less* i)          = 0;
			virtual void visit(Instruction_cmp_le* i)            = 0;
			virtual void visit(Instruction_cmp_eq* i)            = 0;
			virtual void visit(Instruction_cnd_jmp_less* i)      = 0;
			virtual void visit(Instruction_cnd_jmp_le* i)        = 0;
			virtual void visit(Instruction_cnd_jmp_eq* i)        = 0;
			virtual void visit(Instruction_at* i)                = 0;
			virtual void visit(Instruction_load* i)              = 0;
			virtual void visit(Instruction_store* i)             = 0;
			virtual void visit(Instruction_call* i)              = 0;
			virtual void visit(Instruction_call_print* i)        = 0;
			virtual void visit(Instruction_call_input* i)        = 0;
			virtual void visit(Instruction_call_allocate* i)     = 0;
			virtual void visit(Instruction_call_tensor_error* i) = 0;
			virtual void visit(Instruction_load_stack_arg* i)    = 0;
	};

}
