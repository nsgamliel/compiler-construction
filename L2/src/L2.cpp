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