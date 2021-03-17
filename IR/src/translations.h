#pragma once

#include <IR.h>

namespace IR {

	void translate(Function* f);

	void translateLengthInstructions(Function* f);
	void translateArrayInitInstructions(Function* f);
	void translateArrayAccessInstructions(Function* f);
	void translateTupleInitInstructions(Function* f);
	void translateTupleAccessInstructions(Function* f);

}