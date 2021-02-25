#include <vector>

#include <L3.h>

namespace L3 {

	/* implementation plan
	 * 
	 * tile abstract parent class - sets up cost, instrnode*, match(InstNode*), accept(TileVisitor* ), other? members
	 * children implement this for specific tiles (start with atomic only)
	 * then make TileVisitor which emits target language instructions for each tile
	 * 
	 * instr selection will build up a vector of tiles (can be local to fcn) then loop through and call visitor 
	 * when constructing tile vectors, need to maintain each node's isTiled member
	 * also ignore isMerged nodes, they exist in a larger tree elsewhere
	 * 
	 * problem: how to check all possible tiles for a match?
	 * AtomicTiler visitor that calls based on each (L3) instruction type
	 * when implementing special tiles, honestly make a vector of special tiles listed by size and iterate thru each one's match() fcn
	 * before continuing with atomic tiles if all else fails
	 * note: maximal munch is only used for special tiles (atomic tiles are determined by instr type for ease)
	 *
	 */

	class TileVisitor;

	class Tile {
		public:
			int cost;
			int nodesCovered;
			InstructionNode* instr;
			virtual bool match(InstructionNode* i) = 0;
			virtual void accept(TileVisitor* tv) = 0;
	};

	/*
	 * atomic tiles
	 */

	class Tile_return : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_return_val : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_mov : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_plus : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_minus : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_times: public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_and : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_lsh : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_rsh : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_eq : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_le : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_ge : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_less : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_greater : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_load : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_store : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_label : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_branch : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_cond_branch : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call_print : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call_allocate : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call_input : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call_tensor_error : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call_assign : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call_allocate_assign : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class Tile_call_input_assign : public Tile {
		public:
			int cost = 0;
			int nodesCovered = 0;
			bool match(InstructionNode* i) override;
			void accept(TileVisitor* tv) override;
	};

	class InstructionTiler {
		public:
			// vector of tiles, special tiles ordered by number of nodes covered
			std::vector<Tile*> atomicTiles = {
				new Tile_return(),
				new Tile_return_val(),
				new Tile_mov(),
				new Tile_plus(),
				new Tile_minus(),
				new Tile_times(),
				new Tile_and(),
				new Tile_lsh(),
				new Tile_rsh(),
				new Tile_eq(),
				new Tile_le(),
				new Tile_ge(),
				new Tile_less(),
				new Tile_greater(),
				new Tile_load(),
				new Tile_store(),
				new Tile_label(),
				new Tile_branch(),
				new Tile_cond_branch(),
				new Tile_call(),
				new Tile_call_print(),
				new Tile_call_allocate(),
				new Tile_call_input(),
				new Tile_call_tensor_error(),
				new Tile_call_assign(),
				new Tile_call_allocate_assign(),
				new Tile_call_input_assign()
			};
	};

	class TileVisitor {
		public:
			virtual void visit(Tile_return* t) = 0;
			virtual void visit(Tile_return_val* t) = 0;
			virtual void visit(Tile_mov* t) = 0;
			virtual void visit(Tile_plus* t) = 0;
			virtual void visit(Tile_minus* t) = 0;
			virtual void visit(Tile_times* t) = 0;
			virtual void visit(Tile_and* t) = 0;
			virtual void visit(Tile_lsh* t) = 0;
			virtual void visit(Tile_rsh* t) = 0;
			virtual void visit(Tile_eq* t) = 0;
			virtual void visit(Tile_le* t) = 0;
			virtual void visit(Tile_ge* t) = 0;
			virtual void visit(Tile_less* t) = 0;
			virtual void visit(Tile_greater* t) = 0;
			virtual void visit(Tile_load* t) = 0;
			virtual void visit(Tile_store* t) = 0;
			virtual void visit(Tile_label* t) = 0;
			virtual void visit(Tile_branch* t) = 0;
			virtual void visit(Tile_cond_branch* t) = 0;
			virtual void visit(Tile_call* t) = 0;
			virtual void visit(Tile_call_print* t) = 0;
			virtual void visit(Tile_call_allocate* t) = 0;
			virtual void visit(Tile_call_input* t) = 0;
			virtual void visit(Tile_call_tensor_error* t) = 0;
			virtual void visit(Tile_call_assign* t) = 0;
			virtual void visit(Tile_call_allocate_assign* t) = 0;
			virtual void visit(Tile_call_input_assign* t) = 0;
	};

	class L2FromTileGenerator : public TileVisitor {
		public:
			void visit(Tile_return* t) override;
			void visit(Tile_return_val* t) override;
			void visit(Tile_mov* t) override;
			void visit(Tile_plus* t) override;
			void visit(Tile_minus* t) override;
			void visit(Tile_times* t) override;
			void visit(Tile_and* t) override;
			void visit(Tile_lsh* t) override;
			void visit(Tile_rsh* t) override;
			void visit(Tile_eq* t) override;
			void visit(Tile_le* t) override;
			void visit(Tile_ge* t) override;
			void visit(Tile_less* t) override;
			void visit(Tile_greater* t) override;
			void visit(Tile_load* t) override;
			void visit(Tile_store* t) override;
			void visit(Tile_label* t) override;
			void visit(Tile_branch* t) override;
			void visit(Tile_cond_branch* t) override;
			void visit(Tile_call* t) override;
			void visit(Tile_call_print* t) override;
			void visit(Tile_call_allocate* t) override;
			void visit(Tile_call_input* t) override;
			void visit(Tile_call_tensor_error* t) override;
			void visit(Tile_call_assign* t) override;
			void visit(Tile_call_allocate_assign* t) override;
			void visit(Tile_call_input_assign* t) override;
	};

}