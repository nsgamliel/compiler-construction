#include <IR.h>

namespace IR {

	/*
	 * items
	 */

	Label::Label(const std::string& str) {
		name = str;
	}

	Label* Label::getDup(std::vector<Label*> lbls) {
		for (auto lbl : lbls) {
			if (lbl->name.compare(this->name) == 0) return lbl;
		}
		return nullptr;
	}

	Variable::Variable(const std::string& str) {
		name = str;
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

	Instruction_define_var::Instruction_define_var(std::string t, Variable* v) {
		type = t;
		var = v;
	}
	
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

	Instruction_from_array::Instruction_from_array(Variable* s, std::vector<Item*> i, Variable* d) {
		src = s;
		indices = i;
		dst = d;
	}

	Instruction_to_array::Instruction_to_array(Variable* s, std::vector<Item*> i, Variable* d) {
		src = s;
		indices = i;
		dst = d;
	}

	Instruction_length::Instruction_length(Variable* s, Item* di, Variable* d) {
		src = s;
		dim = di;
		dst = d;
	}

	Instruction_label::Instruction_label(Label* l) {
		label = l;
	}

	Instruction_branch::Instruction_branch(Label* l) {
		label = l;
	}

	Instruction_cond_branch::Instruction_cond_branch(Variable* c, Label* t, Label* f) {
		cond = c;
		labelTrue = t;
		labelFalse = f;
	}

	Instruction_call::Instruction_call(Item* d, std::vector<Item*> a) {
		dst = d;
		args = a;
	}

	Instruction_call_print::Instruction_call_print(Item* a) {
		arg = a;
	}

	Instruction_call_tensor_error::Instruction_call_tensor_error(std::vector<Item*> a) {
		args = a;
	}

	Instruction_call_assign::Instruction_call_assign(Item* s, std::vector<Item*> a, Variable* d) {
		src = s;
		args = a;
		dst = d;
	}

	Instruction_call_print_assign::Instruction_call_print_assign(Item* a, Variable* d) {
		arg = a;
		dst = d;
	}

	Instruction_call_input_assign::Instruction_call_input_assign(Variable* d) {
		dst = d;
	}

	Instruction_call_tensor_error_assign::Instruction_call_tensor_error_assign(std::vector<Item*> a, Variable* d) {
		args = a;
		dst = d;
	}

	Instruction_array_init::Instruction_array_init(std::vector<Item*> a, Variable* d) {
		args = a;
		dst = d;
	}

	Instruction_tuple_init::Instruction_tuple_init(Item* a, Variable* d) {
		args = a;
		dst = d;
	}

	void Instruction_return::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_return_val::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_define_var::accept(Visitor* v) { v->visit(this); return; }
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
	void Instruction_from_array::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_to_array::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_length::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_label::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_branch::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_cond_branch::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_print::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_input::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_tensor_error::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_assign::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_print_assign::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_input_assign::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_tensor_error_assign::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_array_init::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_tuple_init::accept(Visitor* v) { v->visit(this); return; }

	/*
	 * program
	 */

}