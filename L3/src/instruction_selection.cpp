#include <iostream>
#include <vector>

#include <instruction_selection.h>
#include <tiler.h>

namespace L3 {

	bool printIS = false;
	// if (printIS) std::cout << "" << std::endl;

	void L3::Function::selectTargetInstructions() {
		this->_treeMerge();
		if (printIS) {
			for (auto c : this->contexts) {
				for (auto t : c->trees) {
					std::cout << "ismerged: " << t->isMerged << std::endl;
				}
			}
		}
		// maximal munch
		auto it = new InstructionTiler();
		Tile* candidateTile;
		for (auto c : this->contexts) {
			for (auto t : c->trees) {
				if (!(t->isTiled)) {
					candidateTile = nullptr;
					// if no special tile found, find the appropriate atomic tile (maximal munch doesn't really matter)
					for (auto tile : it->atomicTiles) {
						if (tile->match(t)) {
							candidateTile = tile;
							break;
						}
					}
					t->tile = candidateTile;
					t->isTiled = true;
				}
			}
		}

		return;
	}

	void L3::Function::_treeMerge() {
		for (auto c : this->contexts) {
			for(int i=0; i<c->trees.size()-1; i++) { // select T2
				for (int j=i+1; j<c->trees.size(); j++) { // although the conditions would likely take care of this, limit T1 to instructions after T2
					int mergeIndex = c->trees[i]->matchLeaf(c->trees[j]);
					if (mergeIndex != -1 && !(dynamic_cast<Variable*>(c->trees[i]->head)->getDup(c->trees[j]->instr->out))) { // ie if match && not in out set (dead), I and IIA
						bool mergeable = true;
						if (j-i > 1) {
							for (auto k=i+1; k<j; k++) {
								// check no other uses of matched var between T2 and T1
								if (dynamic_cast<Variable*>(c->trees[k]->head) && dynamic_cast<Variable*>(c->trees[k]->head)->name.compare(dynamic_cast<Variable*>(c->trees[i]->head)->name) == 0) {
									mergeable = false;
									break;
								}
								if (c->trees[i]->matchLeaf(c->trees[k]) != -1) {
									mergeable = false;
									break;
								}
								// check no definitions of vars used by T2 between T2 and T1
								if (c->trees[k]->matchLeaf(c->trees[i]) != -1) {
									mergeable = false;
									break;
								}
							}
						}
						// merge trees
						if (mergeable) {
							c->trees[j]->leaves[mergeIndex] = c->trees[i];
							c->trees[i]->isMerged = true;
						}
					}
				}
			}
		}
	}

	int L3::InstructionNode::matchLeaf(InstructionNode* t1) {
		// searches through t1's leaves for a match, returning matched leaf index or -1
		if (dynamic_cast<Variable*>(this->head)) {
			for (int i=0; i<t1->leaves.size(); i++) {
				if (dynamic_cast<Variable*>(t1->leaves[i]->head) && dynamic_cast<Variable*>(t1->leaves[i]->head)->name.compare(dynamic_cast<Variable*>(this->head)->name) == 0) {
					return i;
				}
			}
		}
		return -1;
	}
	
}