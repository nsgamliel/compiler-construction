#include <iostream>

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

	Instruction_from_array::Instruction_from_array(bool iT, Variable* s, std::vector<Item*> i, Variable* d) {
		isTup = iT;
		src = s;
		indices = i;
		dst = d;
	}

	Instruction_to_array::Instruction_to_array(bool iT, Item* s, std::vector<Item*> i, Variable* d) {
		isTup = iT;
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

	Instruction_cond_branch::Instruction_cond_branch(Item* c, Label* t, Label* f) {
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


	

	Instruction_load::Instruction_load(Variable* s, Variable* d) {
		src = s;
		dst = d;
	}

	Instruction_store::Instruction_store(Item* s, Variable* d) {
		src = s;
		dst = d;
	}

	Instruction_call_allocate_assign::Instruction_call_allocate_assign(Item* n, Item* v, Variable* d) {
		num = n;
		val = v;
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
	
	void Instruction_load::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_store::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_call_allocate_assign::accept(Visitor* v) { v->visit(this); return; }





	std::string Instruction_return::toL3String() {
		return "return";
	}

	std::string Instruction_return_val::toL3String() {
		return "return " + retValue->toString();
	}

	std::string Instruction_define_var::toL3String() {
			return "";
	}

	std::string Instruction_mov::toL3String() {
		return dst->name + " <- " + src->toString();
	}

	std::string Instruction_plus::toL3String() {
		return dst->name + " <- " + left->toString() + " + " + right->toString();
	}

	std::string Instruction_minus::toL3String() {
		return dst->name + " <- " + left->toString() + " - " + right->toString();
	}

	std::string Instruction_times::toL3String() {
		return dst->name + " <- " + left->toString() + " * " + right->toString();
	}

	std::string Instruction_and::toL3String() {
		return dst->name + " <- " + left->toString() + " & " + right->toString();
	}

	std::string Instruction_lsh::toL3String() {
		return dst->name + " <- " + left->toString() + " << " + right->toString();
	}

	std::string Instruction_rsh::toL3String() {
		return dst->name + " <- " + left->toString() + " >> " + right->toString();
	}

	std::string Instruction_eq::toL3String() {
		return dst->name + " <- " + left->toString() + " = " + right->toString();
	}

	std::string Instruction_le::toL3String() {
		return dst->name + " <- " + left->toString() + " <= " + right->toString();
	}

	std::string Instruction_ge::toL3String() {
		return dst->name + " <- " + left->toString() + " >= " + right->toString();
	}

	std::string Instruction_less::toL3String() {
		return dst->name + " <- " + left->toString() + " < " + right->toString();
	}

	std::string Instruction_greater::toL3String() {
		return dst->name + " <- " + left->toString() + " > " + right->toString();
	}

	std::string Instruction_from_array::toL3String() {
		/*std::string str = dst->name + " <- " + src->name;
		for (auto ind : indices) {
			str += "[" + ind->toString() + "]";
		}
		return str;*/
		return "// ARRAY ACCESSES SHOULD NO LONGER BE PRESENT";
	}

	std::string Instruction_to_array::toL3String() {
		/*std::string str = dst->name;
		for (auto ind : indices) {
			str += "[" + ind->toString() + "]";
		}
		str += " <- " + src->name;
		return str;*/
		return "// ARRAY ACCESSES SHOULD NO LONGER BE PRESENT";
	}

	std::string Instruction_length::toL3String() {
		return "// LENGTH INSTRUCTIONS SHOULD NO LONGER BE PRESENT";
	}

	std::string Instruction_label::toL3String() {
		return label->name;
	}

	std::string Instruction_branch::toL3String() {
		return "br " + label->name;
	}

	std::string Instruction_cond_branch::toL3String() {
		return "br " + cond->toString() + " " + labelTrue->name + "\n\tbr " + labelFalse->name;
	}

	std::string Instruction_call::toL3String() {
		std::string str = "call " + dst->toString() + "(";
		for (int i=0; i<args.size(); i++) {
			str += args[i]->toString();
			if (i != args.size()-1) { str += ", "; }
		} 
		str += ")";
		return str;
	}

	std::string Instruction_call_print::toL3String() {
		return "call print (" + arg->toString() + ")";
	}

	std::string Instruction_call_input::toL3String() {
		return "call input ()";
	}

	std::string Instruction_call_tensor_error::toL3String() {
		std::string str = "call tensor-error (";
		for (int i=0; i<args.size(); i++) {
			str += args[i]->toString();
			if (i != args.size()-1) { str += ", "; }
		} 
		str += ")";
		return str;
	}

	std::string Instruction_call_assign::toL3String() {
		std::string str = dst->name + " <- call " + src->toString() + "(";
		for (int i=0; i<args.size(); i++) {
			str += args[i]->toString();
			if (i != args.size()-1) { str += ", "; }
		} 
		str += ")";
		return str;
	}

	std::string Instruction_call_print_assign::toL3String() {
		return dst->name + " <- call print (" + arg->toString() + ")";
	}

	std::string Instruction_call_input_assign::toL3String() {
		return dst->name + " <- call input ()";
	}

	std::string Instruction_call_tensor_error_assign::toL3String() {
		std::string str = dst->name + " <- call tensor-error (";
		for (int i=0; i<args.size(); i++) {
			str += args[i]->toString();
			if (i != args.size()-1) { str += ", "; }
		} 
		str += ")";
		return str;
	}

	std::string Instruction_array_init::toL3String() {
		return "// ARRAY INITS SHOULD NO LONGER BE PRESENT";
	}

	std::string Instruction_tuple_init::toL3String() {
		return "// TUPLE INITS SHOULD NO LONGER BE PRESENT";
	}

	
	std::string Instruction_load::toL3String() {
		return dst->name + " <- load " + src->name;
	}

	std::string Instruction_store::toL3String() {
		return "store " + dst->name + " <- " + src->toString();
	}

	std::string Instruction_call_allocate_assign::toL3String() {
		return dst->toString() + " <- call allocate (" + num->toString() + ", " + val->toString() + ")";
	}





	/*
	 * program
	 */

}