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
				if (!(c->isLabel)) {
					for (auto t : c->trees) {
						std::cout << "ismerged: " << t->isMerged << std::endl;
					}
				}
			}
		}
		// maximal munch
		if (printIS) std::cout << "begin tiling" << std::endl;
		auto it = new InstructionTiler();
		Tile* candidateTile;
		for (auto c : this->contexts) {
			if (printIS) std::cout << "in context" << std::endl;
			for (auto t : c->trees) {
				if (printIS) std::cout << "in tree" << std::endl;
				if (!(t->isTiled)) {
					if (printIS) std::cout << "not tiled" << std::endl;
					candidateTile = nullptr;
					// if no special tile found, find the appropriate atomic tile (maximal munch doesn't really matter)
					for (auto tile : it->atomicTiles) {
						if (printIS) std::cout << "checking tile candidate" << std::endl;
						if (tile->match(t)) {
							if (printIS) std::cout << "match!" << std::endl;
							candidateTile = tile;
							break;
						}
					}
					if (printIS) std::cout << "assigning tile" << std::endl;
					// duplicate the candidate tile, give it the current instruction, and reassign
					auto newTile = candidateTile->clone();
					if (printIS) std::cout << "candidate cloned" << std::endl;
					newTile->instr = t->instr;
					if (printIS) std::cout << "instruction ptr assigned" << std::endl;
					t->tile = newTile;
					if (printIS) std::cout << "tile ptr assigned" << std::endl;
					t->isTiled = true;
					if (printIS) std::cout << "done assigning tile" << std::endl;
				}
			}
		}
		// now that all tiles are assigned, create a vector of ordered tiles
		std::vector<Tile*> tiles;
		for (auto c : this->contexts) {
			if (printIS) std::cout << "in context" << std::endl;
			for (auto t : c->trees) {
				if (printIS) std::cout << "in tree" << std::endl;
				// call a recursive function to build all tiles in current tree
				if (!(t->isMerged)) {
					if (printIS) std::cout << "not merged" << std::endl;
					auto treeTiles = extractTiles(t);
					for (int i=treeTiles.size()-1; i>=0; i--) {
						if (printIS) std::cout << "\n" << treeTiles[i]->generateTargetInstructions() << "\n\n";
						tiles.push_back(treeTiles[i]);
					}
				}
			}
		}
		this->tiles = tiles;
		return;
	}

	std::vector<Tile*> extractTiles(InstructionNode* i) {
		std::vector<Tile*> tiles;
		if (i->isTiled) {
			tiles.push_back(i->tile);
		}
		if (!i->isLeaf) {
			std::vector<Tile*> tileVec;
			for (auto leaf : i->leaves) {
				tileVec = extractTiles(leaf);
			}
			for (auto tile : tileVec) {
				tiles.push_back(tile);
			}
		}
		return tiles;
	}

	void L3::Function::_treeMerge() {
		for (auto c : this->contexts) {
			if (c->trees.size() > 1) {
				for(int i=0; i<c->trees.size()-1; i++) { // select T2
					if (!(c->isLabel)) {
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