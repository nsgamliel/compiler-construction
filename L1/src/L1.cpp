#include <L1.h>

namespace L1 {

	/*
	 * items
	 */



	/*
	 * instructions
	 */

	void Instruction_return::accept(Visitor* v) { v->visit(this); return; }

}