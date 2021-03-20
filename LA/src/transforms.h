#pragma once

#include <LA.h>

namespace LA {

	void transform(Function* f);

	void encodeDecode(Function* f);
	void checkAllocationAccess(Function* f);
	void buildBasicBlocks(Function* f);

}