#include <iostream>

#include <tracing.h>

namespace IR {

	bool printT = true;
	// if (printT) std::cout << "" << std::endl;

	void traceFunction(Function* f) {
		if (printT) std::cout << "current num bbs: " << f->bbs.size() << std::endl;
		findBasicBlockSuccessors(f);
		std::vector<BasicBlock*> bbsList = f->bbs;
		std::vector<BasicBlock*> traced;

		while (bbsList.size() > 0) {
			if (printT) std::cout << "bbslist size: " << bbsList.size() << std::endl;
			auto fetched = findAndRemove(&bbsList);
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
		if (printT) std::cout << "num traced bbs: " << traced.size() << std::endl;
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

	BasicBlock* findAndRemove(std::vector<BasicBlock*>* bbs) {
		BasicBlock* selection = nullptr;
		int selectionIndex = -1;
		// return first basicblock immediately if not marked
		if (bbs->at(0) && !(bbs->at(0)->isMarked)) {
			selection = bbs->at(0);
			bbs->at(0) = nullptr;
			bbs->erase(bbs->begin());
			return selection;
		}
		// otherwise, find the next basicblock with fewest successors
		for (int i=0; i<bbs->size(); i++) {
			if (!selection || (bbs->at(i) && (selection->succs.size() > bbs->at(i)->succs.size()))) {
				selection = bbs->at(i);
				selectionIndex = i;
			}
		}
		bbs->erase(bbs->begin()+selectionIndex);
		std::cout << "vector size: " << bbs->size() << std::endl;
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