#include <register_allocation.h>
#include <graph_coloring.h>

namespace L2 {

	RegisterAllocator::RegisterAllocator(Function* f) {
		f_working = f;
		f_orig = f->clone();
	}

	Function* RegisterAllocator::allocate_registers() {
		// code analysis
		this->code_analysis();

		// setup

		//graph coloring
		do {

		} while (!(ig->isColored));

		return f_working;

	}

	void RegisterAllocator::code_analysis() {
		f_working->generate_liveness();
		ig = new InterferenceGraph(f_working);
		return;
	}

}