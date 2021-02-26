#include <L3.h>

namespace L3 {

	/*
	 * items
	 */

	Label::Label(const std::string& str) {
		name = str;
	}

	Variable::Variable(const std::string& str) {
		name = str;
	}

	Variable* Variable::getDup(std::vector<Variable*> vec) {
		for(auto var : vec) {
			if (this->name.compare(var->name) == 0) return var;
		}
		return nullptr;
	}

	Number::Number(int64_t num) {
		value = num;
	}

	std::string Label::toString() {
		return this->name;
	}

	std::string Variable::toString() {
		return this->name;
	}

	std::string Number::toString() {
		return std::to_string(this->value);
	}

	/*
	 * instructions
	 */
	
	Instruction_return_val::Instruction_return_val(Item* rv) {
		retValue = rv;
	}

	Instruction_mov::Instruction_mov(Item* s, Variable* d) {
		src = s;
		dst = d;
	}

	Instruction_op::Instruction_op(Item* l, Item* r, Variable* d) {
		left = l;
		right = r;
		dst = d;
	}

	Instruction_plus::Instruction_plus(Item* l, Item* r, Variable* d)
	: Instruction_op(l, r, d) {}

	Instruction_minus::Instruction_minus(Item* l, Item* r, Variable* d)
	: Instruction_op(l, r, d) {}

	Instruction_times::Instruction_times(Item* l, Item* r, Variable* d)
	: Instruction_op(l, r, d) {}

	Instruction_and::Instruction_and(Item* l, Item* r, Variable* d)
	: Instruction_op(l, r, d) {}

	Instruction_lsh::Instruction_lsh(Item* l, Item* r, Variable* d)
	: Instruction_op(l, r, d) {}

	Instruction_rsh::Instruction_rsh(Item* l, Item* r, Variable* d)
	: Instruction_op(l, r, d) {}

	Instruction_cmp::Instruction_cmp(Item* l, Item* r, Variable* d) {
		left = l;
		right = r;
		dst = d;
	}

	Instruction_eq::Instruction_eq(Item* l, Item* r, Variable* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_le::Instruction_le(Item* l, Item* r, Variable* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_ge::Instruction_ge(Item* l, Item* r, Variable* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_less::Instruction_less(Item* l, Item* r, Variable* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_greater::Instruction_greater(Item* l, Item* r, Variable* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_load::Instruction_load(Variable* s, Variable* d) {
		src = s;
		dst = d;
	}

	Instruction_store::Instruction_store(Item* s, Variable* d) {
		src = s;
		dst = d;
	}

	Instruction_label::Instruction_label(Label* l) {
		label = l;
	}

	Instruction_branch::Instruction_branch(Label* l) {
		label = l;
	}

	Instruction_cond_branch::Instruction_cond_branch(Variable* c, Label* l) {
		cond = c;
		label = l;
	}

	Instruction_call::Instruction_call(Item* d, std::vector<Item*> a) {
		dst = d;
		args = a;
	}

	Instruction_call_print::Instruction_call_print(Item* a) {
		arg = a;
	}

	Instruction_call_allocate::Instruction_call_allocate(Item* n, Item* v) {
		num = n;
		val = v;
	}

	Instruction_call_tensor_error::Instruction_call_tensor_error(Number* a) {
		arg = a;
	}

	Instruction_call_assign::Instruction_call_assign(Item* s, std::vector<Item*> a, Variable* d) {
		src = s;
		args = a;
		dst = d;
	}

	Instruction_call_allocate_assign::Instruction_call_allocate_assign(Item* n, Item* v, Variable* d) {
		num = n;
		val = v;
		dst = d;
	}

	Instruction_call_input_assign::Instruction_call_input_assign(Variable* d) {
		dst = d;
	}

	void Instruction_return::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_return_val::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_mov::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_plus::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_minus::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_times::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_and::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_lsh::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_rsh::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_eq::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_le::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_ge::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_less::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_greater::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_load::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_store::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_label::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_branch::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_cond_branch::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_print::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_allocate::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_input::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_tensor_error::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_assign::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_allocate_assign::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_input_assign::accept(Visitor* v) { v->visit(this); return; }

	/*
	 * program
	 */

	void Program::globalizeLabels() {
		int num = 0;
		for (auto f : this->functions) {
			for (int i=0; i<f->instructions.size(); i++) {
				if (dynamic_cast<Instruction_label*>(f->instructions[i])) {
					auto newI = dynamic_cast<Instruction_label*>(f->instructions[i]);
					newI->label = new Label(this->longestLabel + "_" + std::to_string(num) + "_" + newI->label->name.substr(1));
				} else if (dynamic_cast<Instruction_branch*>(f->instructions[i])) {
					auto newI = dynamic_cast<Instruction_branch*>(f->instructions[i]);
					newI->label = new Label(this->longestLabel + "_" + std::to_string(num) + "_" + newI->label->name.substr(1));
				} else if (dynamic_cast<Instruction_cond_branch*>(f->instructions[i])) {
					auto newI = dynamic_cast<Instruction_cond_branch*>(f->instructions[i]);
					newI->label = new Label(this->longestLabel + "_" + std::to_string(num) + "_" + newI->label->name.substr(1));
				} else if (dynamic_cast<Instruction_call*>(f->instructions[i])) {
					auto newI = dynamic_cast<Instruction_call*>(f->instructions[i]);
					if (dynamic_cast<Label*>(newI->dst)) {
						newI->dst = new Label(this->longestLabel + std::to_string(num) + dynamic_cast<Label*>(newI->dst)->name);
					}
				}
			}
			num++;
		}
	}

}