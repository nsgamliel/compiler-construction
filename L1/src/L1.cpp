#include <L1.h>

namespace L1 {

	/*
	 * items
	 */

	

	/*
	 * instructions
	 */

	

	/*
	 * accept overrides
	 */

	void Instruction_return::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_mov::accept         (Visitor* v) { v->visit(this); return; }
	void Instruction_label::accept       (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_pe::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_me::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_te::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_ae::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_pp::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_aop_mm::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_sop_lsh::accept     (Visitor* v) { v->visit(this); return; }
	void Instruction_sop_rsh::accept     (Visitor* v) { v->visit(this); return; }
	void Instruction_dir_jmp::accept     (Visitor* v) { v->visit(this); return; }
	void Instruction_cmp_less::accept    (Visitor* v) { v->visit(this); return; }
	void Instruction_cmp_le::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_cmp_eq::accept      (Visitor* v) { v->visit(this); return; }
	void Instruction_cnd_jmp_less::accept(Visitor* v) { v->visit(this); return; }
	void Instruction_cnd_jmp_le::accept  (Visitor* v) { v->visit(this); return; }
	void Instruction_cnd_jmp_eq::accept  (Visitor* v) { v->visit(this); return; }
	void Instruction_at::accept          (Visitor* v) { v->visit(this); return; }
	void Instruction_load::accept        (Visitor* v) { v->visit(this); return; }
	void Instruction_store::accept       (Visitor* v) { v->visit(this); return; }
	void Instruction_call_local::accept  (Visitor* v) { v->visit(this); return; }
	void Instruction_call_runtime::accept(Visitor* v) { v->visit(this); return; }

}