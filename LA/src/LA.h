#pragma once

#include <map>
#include <string>
#include <vector>

namespace LA {

	class Visitor;

	/*
	 * items
	 */

	class Item {
		public:
			virtual ~Item(){};
			virtual std::string toString() = 0;
	};

	class Label : public Item {
		public:
			Label(const std::string& str);
			Label* getDup(std::vector<Label*> lbls);
			std::string toString() override;
			std::string name;
	};

	class Variable : public Item {
		public:
			Variable(const std::string& str);
			Variable* getDup(std::vector<Variable*> vec);
			std::string toString() override;
			std::string name;
			std::string type;
	};

	class Number : public Item {
		public:
			Number(int64_t num);
			std::string toString() override;
			int64_t value;
	};

	/*
	 * instructions
	 */

	class Instruction {
		public:
			virtual void accept(Visitor* v) = 0;
			virtual std::string toIRString() = 0;
	};

	class Instruction_define_var : public Instruction {
		public:
			Instruction_define_var(std::string t, Variable* v);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			std::string type;
			Variable* var;
	};

	class Instruction_return : public Instruction {
		public:
			void accept(Visitor* v) override;
			std::string toIRString() override;
	};

	class Instruction_return_val : public Instruction {
		public:
			Instruction_return_val(Item* rv);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* retValue;
	};

	class Instruction_mov : public Instruction {
		public:
			Instruction_mov(Item* s, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* src;
			Variable* dst;
	};

	class Instruction_op : public Instruction {
		public:
			Instruction_op(Item* l, Item* r, Variable* d);
			virtual void accept(Visitor* v) = 0;
			virtual std::string toIRString() = 0;
			Item* left;
			Item* right;
			Variable* dst;
			int opcode; // for encoding/decoding
	};

	class Instruction_plus : public Instruction_op {
		public:
			Instruction_plus(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 0;
	};

	class Instruction_minus : public Instruction_op {
		public:
			Instruction_minus(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 1;
	};

	class Instruction_times : public Instruction_op {
		public:
			Instruction_times(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 2;
	};

	class Instruction_and : public Instruction_op {
		public:
			Instruction_and(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 3;
	};

	class Instruction_lsh : public Instruction_op {
		public:
			Instruction_lsh(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 4;
	};

	class Instruction_rsh : public Instruction_op {
		public:
			Instruction_rsh(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 5;
	};

	/*class Instruction_cmp : public Instruction {
		public:
			Instruction_cmp(Item* l, Item* r, Variable* d);
			virtual void accept(Visitor* v) = 0;
			virtual std::string toIRString() = 0;
			Item* left;
			Item* right;
			Variable* dst;
	};*/

	class Instruction_eq : public Instruction_op {
		public:
			Instruction_eq(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 6;
	};

	class Instruction_le : public Instruction_op {
		public:
			Instruction_le(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 7;
	};

	class Instruction_ge : public Instruction_op {
		public:
			Instruction_ge(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 8;
	};

	class Instruction_less : public Instruction_op {
		public:
			Instruction_less(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 9;
	};

	class Instruction_greater : public Instruction_op {
		public:
			Instruction_greater(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			int opcode = 10;
	};
	
	class Instruction_from_array : public Instruction {
		public:
			Instruction_from_array(bool iT, Variable* s, std::vector<Item*> i, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			bool isTup;
			Variable* src;
			std::vector<Item*> indices;
			Variable* dst;
	};
	
	class Instruction_to_array : public Instruction {
		public:
			Instruction_to_array(bool iT, Item* s, std::vector<Item*> i, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			bool isTup;
			Item* src;
			std::vector<Item*> indices;
			Variable* dst;
	};

	class Instruction_length : public Instruction {
		public:
			Instruction_length(Variable* s, Item* di, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Variable* src;
			Item* dim;
			Variable* dst;
	};

	class Instruction_label : public Instruction {
		public:
			Instruction_label(Label* l);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Label* label;
	};

	class Instruction_branch : public Instruction {
		public:
			Instruction_branch(Label* l);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Label* label;
	};

	class Instruction_cond_branch : public Instruction {
		public:
			Instruction_cond_branch(Item* c, Label* t, Label* f);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* cond;
			Label* labelTrue;
			Label* labelFalse;
	};

	class Instruction_call : public Instruction {
		public:
			Instruction_call(Item* d, std::vector<Item*> a);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* dst;
			std::vector<Item*> args;
	};

	class Instruction_call_print : public Instruction {
		public:
			Instruction_call_print(Item* a);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* arg;
	};

	class Instruction_call_input : public Instruction {
		public:
			void accept(Visitor* v) override;
			std::string toIRString() override;
	};

	class Instruction_call_tensor_error : public Instruction {
		public:
			Instruction_call_tensor_error(std::vector<Item*> a);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			std::vector<Item*> args;
	};

	class Instruction_call_assign : public Instruction {
		public:
			Instruction_call_assign(Item* s, std::vector<Item*> a, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* src;
			std::vector<Item*> args;
			Variable* dst;
	};

	class Instruction_call_print_assign : public Instruction {
		public:
			Instruction_call_print_assign(Item* a, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* arg;
			Variable* dst;
	};

	class Instruction_call_input_assign : public Instruction {
		public:
			Instruction_call_input_assign(Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Variable* dst;
	};

	class Instruction_array_init : public Instruction {
		public:
			Instruction_array_init(std::vector<Item*> a, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			std::vector<Item*> args;
			Variable* dst;
	};

	class Instruction_tuple_init : public Instruction {
		public:
			Instruction_tuple_init(Item* a, Variable* d);
			void accept(Visitor* v) override;
			std::string toIRString() override;
			Item* args;
			Variable* dst;
	};

	/*
	 * functions
	 */

	class Function {
		public:			
			std::string name;
			std::string longestVar;
			int numSub = 0;
			std::vector<Variable*> params;
			std::vector<Instruction*> instrs;
	};

	/*
	 * program
	 */

	class Program {
		public:
			std::vector<Function*> functions;
	};

	/*
	 * visitor
	 */

	class Visitor {
		public:
			virtual void visit(Instruction_return* i) = 0;
			virtual void visit(Instruction_return_val* i) = 0;
			virtual void visit(Instruction_define_var* i) = 0;
			virtual void visit(Instruction_mov* i) = 0;
			virtual void visit(Instruction_plus* i) = 0;
			virtual void visit(Instruction_minus* i) = 0;
			virtual void visit(Instruction_times* i) = 0;
			virtual void visit(Instruction_and* i) = 0;
			virtual void visit(Instruction_lsh* i) = 0;
			virtual void visit(Instruction_rsh* i) = 0;
			virtual void visit(Instruction_eq* i) = 0;
			virtual void visit(Instruction_le* i) = 0;
			virtual void visit(Instruction_ge* i) = 0;
			virtual void visit(Instruction_less* i) = 0;
			virtual void visit(Instruction_greater* i) = 0;
			virtual void visit(Instruction_from_array* i) = 0;
			virtual void visit(Instruction_to_array* i) = 0;
			virtual void visit(Instruction_length* i) = 0;
			virtual void visit(Instruction_label* i) = 0;
			virtual void visit(Instruction_branch* i) = 0;
			virtual void visit(Instruction_cond_branch* i) = 0;
			virtual void visit(Instruction_call* i) = 0;
			virtual void visit(Instruction_call_print* i) = 0;
			virtual void visit(Instruction_call_input* i) = 0;
			virtual void visit(Instruction_call_tensor_error* i) = 0;
			virtual void visit(Instruction_call_assign* i) = 0;
			virtual void visit(Instruction_call_print_assign* i) = 0;
			virtual void visit(Instruction_call_input_assign* i) = 0;
			virtual void visit(Instruction_array_init* i) = 0;
			virtual void visit(Instruction_tuple_init* i) = 0;
	};

}