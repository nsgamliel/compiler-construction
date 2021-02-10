#include <iostream>

#include <L2.h>

namespace L2 {

	/*
	 * items
	 */

	Label::Label(const std::string& str) {
		name = str;
	}

	Variable::Variable(const std::string& n) {
		name = n;
	}

	Variable* Variable::get_dup(std::vector<Variable*> vec) {
		for(auto var : vec) {
			if (this->name.compare(var->name) == 0) return var;
		}
		return nullptr;
	}

	bool Variable::is_in(std::vector<Variable*> vec) {
		for(auto var : vec) {
			if (this->name.compare(var->name) == 0) return true;
		}
		return false;
	}

	Register::Register(const std::string& n, RegisterId r)
	: Variable(n) {
		rid = r;
	}

	Number::Number(int64_t n) {
		value = n;
	}

	Memory::Memory(Variable* v, Number* n) {
		var = v;
		offset = n;
	}

	/*
	 * instructions
	 */

	Instruction_op_one::Instruction_op_one(Item* s) {
		src = s;
	}

	Instruction_op_two::Instruction_op_two(Item* s, Item* d) {
		src = s;
		dst = d;
	}

	Instruction_label::Instruction_label(Label* l) {
		label = l;
	}

	Instruction_return::Instruction_return(int64_t n) {
		stack_alloc = n;
	}

	Instruction_mov::Instruction_mov(Item* s, Variable* d)
	: Instruction_op_two(s, d) {}

	Instruction_aop_pe::Instruction_aop_pe(Item* s, Item* d)
	: Instruction_op_two(s, d) {}

	Instruction_aop_me::Instruction_aop_me(Item* s, Item* d)
	: Instruction_op_two(s, d) {}

	Instruction_aop_te::Instruction_aop_te(Item* s, Item* d)
	: Instruction_op_two(s, d) {}

	Instruction_aop_ae::Instruction_aop_ae(Item* s, Item* d)
	: Instruction_op_two(s, d) {}

	Instruction_aop_pp::Instruction_aop_pp(Item* s)
	: Instruction_op_one(s) {}

	Instruction_aop_mm::Instruction_aop_mm(Item* s)
	: Instruction_op_one(s) {}

	Instruction_sop_lsh::Instruction_sop_lsh(Item* s, Item* d)
	: Instruction_op_two(s, d) {}

	Instruction_sop_rsh::Instruction_sop_rsh(Item* s, Item* d)
	: Instruction_op_two(s, d) {}

	Instruction_dir_jmp::Instruction_dir_jmp(Label* l)
	: Instruction_label(l) {}

	Instruction_cmp::Instruction_cmp(Item* l, Item* r, Item* d) {
		left = l;
		right = r;
		dst = d;
	}

	Instruction_cmp_less::Instruction_cmp_less(Item* l, Item* r, Item* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_cmp_le::Instruction_cmp_le(Item* l, Item* r, Item* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_cmp_eq::Instruction_cmp_eq(Item* l, Item* r, Item* d)
	: Instruction_cmp(l, r, d) {}

	Instruction_cnd_jmp::Instruction_cnd_jmp(Item* l, Item* r, Label* d) {
		left = l;
		right = r;
		dst = d;
	}

	Instruction_cnd_jmp_less::Instruction_cnd_jmp_less(Item* l, Item* r, Label* d)
	: Instruction_cnd_jmp(l, r, d) {}

	Instruction_cnd_jmp_le::Instruction_cnd_jmp_le(Item* l, Item* r, Label* d)
	: Instruction_cnd_jmp(l, r, d) {}

	Instruction_cnd_jmp_eq::Instruction_cnd_jmp_eq(Item* l, Item* r, Label* d)
	: Instruction_cnd_jmp(l, r, d) {}

	Instruction_at::Instruction_at(Variable* b, Variable* i, Number* s, Variable* d) {
		base = b;
		index = i;
		scale = s;
		dst = d;
	}

	Instruction_load::Instruction_load(Memory* s, Item* d)
	: Instruction_op_two(s, d) {}

	Instruction_store::Instruction_store(Item* s, Memory* d)
	: Instruction_op_two(s, d) {}

	Instruction_call::Instruction_call(Item* d, Number* a) {
		dst = d;
		args = a;
	}

	Instruction_call_print::Instruction_call_print()
	: Instruction_call(new Label("print"), new Number(1)) {}

	Instruction_call_input::Instruction_call_input()
	: Instruction_call(new Label("input"), new Number(0)) {}

	Instruction_call_allocate::Instruction_call_allocate()
	: Instruction_call(new Label("allocate"), new Number(2)) {}

	Instruction_call_tensor_error::Instruction_call_tensor_error(Number* a)
	: Instruction_call(new Label("tensor-error"), a) {}

	Instruction_load_stack_arg::Instruction_load_stack_arg(Number* s, Item* d) {
		offset = s;
		dst = d;
	}

	/*
	 * toString()
	 */

	std::string Instruction_return::toString() {
		return "return";
	}

	std::string Instruction_mov::toString() {
		return conv_item(dst) + " <- " + conv_item(src);
	}

	std::string Instruction_label::toString() {
		return (dynamic_cast<Label*> (label))->name;
	}

	std::string Instruction_aop_pe::toString() {
		return conv_item(dst) + " += " + conv_item(src);
	}

	std::string Instruction_aop_me::toString() {
		return conv_item(dst) + " -= " + conv_item(src);
	}

	std::string Instruction_aop_te::toString() {
		return conv_item(dst) + " *= " + conv_item(src);
	}

	std::string Instruction_aop_ae::toString() {
		return conv_item(dst) + " &= " + conv_item(src);
	}

	std::string Instruction_aop_pp::toString() {
		return conv_item(src) + "++";
	}

	std::string Instruction_aop_mm::toString() {
		return conv_item(src) + "--";
	}

	std::string Instruction_sop_lsh::toString() {
		return conv_item(dst) + " <<= " + conv_item(src);
	}

	std::string Instruction_sop_rsh::toString() {
		return conv_item(dst) + " >>= " + conv_item(src);
	}

	std::string Instruction_dir_jmp::toString() {
		return "goto " + (dynamic_cast<Label*>(label))->name;
	}

	std::string Instruction_cmp_less::toString() {
		return conv_item(dst) + " <- " + conv_item(left) + " < " + conv_item(right);
	}

	std::string Instruction_cmp_le::toString() {
		return conv_item(dst) + " <- " + conv_item(left) + " <= " + conv_item(right);
	}

	std::string Instruction_cmp_eq::toString() {
		return conv_item(dst) + " <- " + conv_item(left) + " = " + conv_item(right);
	}

	std::string Instruction_cnd_jmp_less::toString() {
		return "cjump " + conv_item(left) + " < " + conv_item(right) + " " + conv_item(dst);
	}

	std::string Instruction_cnd_jmp_le::toString() {
		return "cjump " + conv_item(left) + " <= " + conv_item(right) + " " + conv_item(dst);
	}

	std::string Instruction_cnd_jmp_eq::toString() {
		return "cjump " + conv_item(left) + " = " + conv_item(right) + " " + conv_item(dst);
	}

	std::string Instruction_at::toString() {
		return conv_item(dst) + " @ " + conv_item(base) + " " + conv_item(index) + " " + conv_item(scale);
	}

	std::string Instruction_load::toString() {
		return conv_item(dst) + " <- " + conv_item(src);
	}

	std::string Instruction_store::toString() {
		return conv_item(dst) + " <- " + conv_item(src);
	}

	std::string Instruction_call::toString() {
		return "call " + conv_item(dst) + " " + conv_item(args);
	}

	std::string Instruction_call_print::toString() {
		return "call print 1";
	}

	std::string Instruction_call_input::toString() {
		return "call input 0";
	}

	std::string Instruction_call_allocate::toString() {
		return "call allocate 2";
	}

	std::string Instruction_call_tensor_error::toString() {
		return "call tensor-error " + conv_item(args);
	}

	std::string Instruction_load_stack_arg::toString() {
		return conv_item(dst) + " <- stack-arg " + conv_item(offset);
	}

	std::string conv_item(Item* item) {
		auto item_c = dynamic_cast<Variable*> (item);
		if (item_c) return item_c->name;
		auto item2_c = dynamic_cast<Label*> (item);
		if (item2_c) return item2_c->name;
		auto item3_c = dynamic_cast<Number*> (item);
		if (item3_c) return std::to_string(item3_c->value);
		auto item4_c = dynamic_cast<Memory*> (item);
		if (item4_c) return ("mem " + item4_c->var->name + " " + std::to_string(item4_c->offset->value));
		return "###";
	}

	/*
	 * accept overrides
	 */

	void Instruction_return::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_mov::accept              (Visitor* v) { v->visit(this); return; }
	void Instruction_label::accept            (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_pe::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_me::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_te::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_ae::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_pp::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_mm::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_sop_lsh::accept          (Visitor* v) { v->visit(this); return; }
	void Instruction_sop_rsh::accept          (Visitor* v) { v->visit(this); return; }
	void Instruction_dir_jmp::accept          (Visitor* v) { v->visit(this); return; }
	void Instruction_cmp_less::accept         (Visitor* v) { v->visit(this); return; }
	void Instruction_cmp_le::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_cmp_eq::accept           (Visitor* v) { v->visit(this); return; }
	void Instruction_cnd_jmp_less::accept     (Visitor* v) { v->visit(this); return; }
	void Instruction_cnd_jmp_le::accept       (Visitor* v) { v->visit(this); return; }
	void Instruction_cnd_jmp_eq::accept       (Visitor* v) { v->visit(this); return; }
	void Instruction_at::accept               (Visitor* v) { v->visit(this); return; }
	void Instruction_load::accept             (Visitor* v) { v->visit(this); return; }
	void Instruction_store::accept            (Visitor* v) { v->visit(this); return; }
	void Instruction_call::accept             (Visitor* v) { v->visit(this); return; }
	void Instruction_call_print::accept       (Visitor* v) { v->visit(this); return; }
	void Instruction_call_input::accept       (Visitor* v) { v->visit(this); return; }
	void Instruction_call_allocate::accept    (Visitor* v) { v->visit(this); return; }
	void Instruction_call_tensor_error::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_load_stack_arg::accept   (Visitor* v) { v->visit(this); return; }

}