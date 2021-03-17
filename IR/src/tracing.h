#pragma once

#include <IR.h>

namespace IR {

	void traceFunction(Function* f);

	// helpers
	void findBasicBlockSuccessors(Function* f);
	BasicBlock* findAndRemove(std::vector<BasicBlock*>* bbs);
	bool isProfitable(BasicBlock* bb, BasicBlock* bbSuc);

}