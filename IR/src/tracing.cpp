#include <tracing.h>

namespace IR {

	void traceFunction(Function* f) {
		findBasicBlockSuccessors(f);
		std::vector<BasicBlock*> bbsList = f->bbs;
		int bbsListSize = bbsList.size();
		std::vector<BasicBlock*> traced;

		while (bbsListSize > 0) {
			auto fetched = findAndRemove(bbsList);
			bbsListSize--;
			while (!(fetched->isMarked)) {
				fetched->isMarked = true;
				traced.push_back(fetched);
				for (auto bb : fetched->succs) {
					if (!(bb->isMarked) && isProfitable(fetched, bb)) {
						fetched = bb;
						break;
					}
				}
			}
		}
		f->bbsTraced = traced;
		return;
	}

	void findBasicBlockSuccessors(Function* f) {
		for (auto bb : f->bbs) {
			if (bb->trueSucc) {
				for (auto bbSuc : f->bbs) {
					if (bbSuc != bb && bbSuc->entryLabel == bb->trueSucc) {
						bb->succs.push_back(bbSuc);
					}
				}
			}
			if (bb->falseSucc) {
				for (auto bbSuc : f->bbs) {
					if (bbSuc != bb && bbSuc->entryLabel == bb->falseSucc) {
						bb->succs.push_back(bbSuc);
					}
				}
			}
		}
		return;
	}

	BasicBlock* findAndRemove(std::vector<BasicBlock*> bbs) {
		BasicBlock* selection = nullptr;
		int selectionIndex = -1;
		// return first basicblock immediately if not marked
		if (bbs[0] && !(bbs[0]->isMarked)) {
			selection = bbs[0];
			bbs[0] = nullptr;
			return selection;
		}
		// otherwise, find the next basicblock with fewest successors
		for (int i=0; i<bbs.size(); i++) {
			if (!selection || (bbs[i] && (selection->succs.size() > bbs[i]->succs.size()))) {
				selection = bbs[i];
				selectionIndex = i;
			}
		}
		bbs[selectionIndex] = nullptr;
		return selection;
	}

	bool isProfitable(BasicBlock* bb, BasicBlock* bbSuc) {
		// isprofitable iff the given successor is bb's only successor
		// and one of the successor's successors is the given successor
		// i.e. the basic blocks form a loop
		if (bb->succs.size() == 1 && (bb->trueSucc == bbSuc->entryLabel) &&
		   (bbSuc->trueSucc == bb->entryLabel || bbSuc->falseSucc == bb->entryLabel)) {
			return true;
		}
		return false;
	}

}