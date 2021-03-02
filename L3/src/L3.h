#pragma once

#include <map>
#include <string>
#include <vector>

namespace L3 {

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
			std::string toString() override;
			std::string name;
	};

	class Variable : public Item {
		public:
			Variable(const std::string& str);
			Variable* getDup(std::vector<Variable*> vec);
			std::string toString() override;
			std::string name;
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

			std::vector<Variable*> gen;
			std::vector<Variable*> kill;
			std::vector<Variable*> in;
			std::vector<Variable*> out;
			std::vector<Instruction*> successors;
	};

	class Instruction_return : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_return_val : public Instruction {
		public:
			Instruction_return_val(Item* rv);
			void accept(Visitor* v) override;
			Item* retValue;
	};

	class Instruction_mov : public Instruction {
		public:
			Instruction_mov(Item* s, Variable* d);
			void accept(Visitor* v) override;
			Item* src;
			Variable* dst;
	};

	class Instruction_op : public Instruction {
		public:
			Instruction_op(Item* l, Item* r, Variable* d);
			virtual void accept(Visitor* v) = 0;
			Item* left;
			Item* right;
			Variable* dst;
	};

	class Instruction_plus : public Instruction_op {
		public:
			Instruction_plus(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_minus : public Instruction_op {
		public:
			Instruction_minus(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_times : public Instruction_op {
		public:
			Instruction_times(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_and : public Instruction_op {
		public:
			Instruction_and(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_lsh : public Instruction_op {
		public:
			Instruction_lsh(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_rsh : public Instruction_op {
		public:
			Instruction_rsh(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_cmp : public Instruction {
		public:
			Instruction_cmp(Item* l, Item* r, Variable* d);
			virtual void accept(Visitor* v) = 0;
			Item* left;
			Item* right;
			Variable* dst;
	};

	class Instruction_eq : public Instruction_cmp {
		public:
			Instruction_eq(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_le : public Instruction_cmp {
		public:
			Instruction_le(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_ge : public Instruction_cmp {
		public:
			Instruction_ge(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_less : public Instruction_cmp {
		public:
			Instruction_less(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_greater : public Instruction_cmp {
		public:
			Instruction_greater(Item* l, Item* r, Variable* d);
			void accept(Visitor* v) override;
	};

	class Instruction_load : public Instruction {
		public:
			Instruction_load(Variable* s, Variable* d);
			void accept(Visitor* v) override;
			Variable* src;
			Variable* dst;
	};

	class Instruction_store : public Instruction {
		public:
			Instruction_store(Item* s, Variable* d);
			void accept(Visitor* v) override;
			Item* src;
			Variable* dst;
	};

	class Instruction_label : public Instruction {
		public:
			Instruction_label(Label* l);
			void accept(Visitor* v) override;
			Label* label;
	};

	class Instruction_branch : public Instruction {
		public:
			Instruction_branch(Label* l);
			void accept(Visitor* v) override;
			Label* label;
	};

	class Instruction_cond_branch : public Instruction {
		public:
			Instruction_cond_branch(Variable* c, Label* l);
			void accept(Visitor* v) override;
			Variable* cond;
			Label* label;
	};

	class Instruction_call : public Instruction {
		public:
			Instruction_call(Item* d, std::vector<Item*> a);
			void accept(Visitor* v) override;
			Item* dst;
			std::vector<Item*> args;
	};

	class Instruction_call_print : public Instruction {
		public:
			Instruction_call_print(Item* a);
			void accept(Visitor* v) override;
			Item* arg;
	};

	class Instruction_call_allocate : public Instruction {
		public:
			Instruction_call_allocate(Item* n, Item* v);
			void accept(Visitor* v) override;
			Item* num;
			Item* val;
	};

	class Instruction_call_input : public Instruction {
		public:
			void accept(Visitor* v) override;
	};

	class Instruction_call_tensor_error : public Instruction {
		public:
			Instruction_call_tensor_error(std::vector<Item*> a);
			void accept(Visitor* v) override;
			std::vector<Item*> args;
	};

	class Instruction_call_assign : public Instruction {
		public:
			Instruction_call_assign(Item* s, std::vector<Item*> a, Variable* d);
			void accept(Visitor* v) override;
			Item* src;
			std::vector<Item*> args;
			Variable* dst;
	};

	class Instruction_call_print_assign : public Instruction {
		public:
			Instruction_call_print_assign(Item* a, Variable* d);
			void accept(Visitor* v) override;
			Item* arg;
			Variable* dst;
	};

	class Instruction_call_allocate_assign : public Instruction {
		public:
			Instruction_call_allocate_assign(Item* n, Item* v, Variable* d);
			void accept(Visitor* v) override;
			Item* num;
			Item* val;
			Variable* dst;
	};

	class Instruction_call_input_assign : public Instruction {
		public:
			Instruction_call_input_assign(Variable* d);
			void accept(Visitor* v) override;
			Variable* dst;
	};

	class Instruction_call_tensor_error_assign : public Instruction {
		public:
			Instruction_call_tensor_error_assign(std::vector<Item*> a, Variable* d);
			void accept(Visitor* v) override;
			std::vector<Item*> args;
			Variable* dst;
	};

	/*
	 * instruction trees
	 */

	//enum Operation {};

	/*struct InstructionTree {
		Item* head;
		Operation op;
		std::vector<Item*> leaves;
	};

	struct Context {
		std::vector<InstructionTree*> trees;
	};*/

	class Tile;
	// usage: only instruction destinations get instr and leaves fields populated, other nodes have those fields left blank
	struct InstructionNode {
		Item* head;
		Instruction* instr;
		Tile* tile;
		std::vector<InstructionNode*> leaves;
		bool isLeaf = true; // if so, does not get instr ptr or tile ptr
		bool isMerged = false; // ie disregard this tree if this is true at the top level; it belongs to a bigger tree elsewhere
		bool isTiled = false;
		bool isMergeable = true;
		bool isSpecialChild = false;
		int matchLeaf(InstructionNode* t1);
	};

	// stores each tree's top-level node
	struct Context {
		std::vector<InstructionNode*> trees;
		bool isLabel = false;
	};

	/*
	 * functions
	 */

	class Function {
		public:
			void generateLiveness(); // L3 re-implementation
			void selectTargetInstructions();
			
			std::string name;
			std::vector<Variable*> params;
			std::vector<Instruction*> instructions;
			std::vector<Context*> contexts;
			std::vector<Tile*> tiles; // ordered tiles vector
			// used for maintaining unique variable pointers
			std::vector<Variable*> vars;
			bool isDirty;

		private:
			// helpers
			void _genKill();
			void _findSuccessors();
			void _inOut();
			void _treeMerge();
	};

	/*
	 * program
	 */

	class Program {
		public:
			std::vector<Function*> functions;
			std::string longestLabel;

			void globalizeLabels();
	};

	/*
	 * visitor
	 */

	class Visitor {
		public:
			virtual void visit(Instruction_return* i) = 0;
			virtual void visit(Instruction_return_val* i) = 0;
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
			virtual void visit(Instruction_load* i) = 0;
			virtual void visit(Instruction_store* i) = 0;
			virtual void visit(Instruction_label* i) = 0;
			virtual void visit(Instruction_branch* i) = 0;
			virtual void visit(Instruction_cond_branch* i) = 0;
			virtual void visit(Instruction_call* i) = 0;
			virtual void visit(Instruction_call_print* i) = 0;
			virtual void visit(Instruction_call_allocate* i) = 0;
			virtual void visit(Instruction_call_input* i) = 0;
			virtual void visit(Instruction_call_tensor_error* i) = 0;
			virtual void visit(Instruction_call_assign* i) = 0;
			virtual void visit(Instruction_call_print_assign* i) = 0;
			virtual void visit(Instruction_call_allocate_assign* i) = 0;
			virtual void visit(Instruction_call_input_assign* i) = 0;
			virtual void visit(Instruction_call_tensor_error_assign* i) = 0;
	};

}