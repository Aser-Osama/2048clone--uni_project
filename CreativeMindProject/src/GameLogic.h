#pragma once
#include <string>
#include <vector>
namespace GameLogic
{
	enum class Direction {
		none, up, down, left, right
	};

	struct tilePosHistory {
		uint8_t oldTile_I = -1;
		uint8_t oldTile_J = -1;
		uint8_t Tile_I;
		uint8_t Tile_J;		
		bool justMoved = false;
		bool justMerged = false;
		bool madeFirstMove = false;		

	};

	class Tile {
	private:
		static uint16_t tileID; // Unused but could be helpful for the score.
		uint8_t tileVal = 0;
		uint8_t oldTileVal = 0;
		tilePosHistory Position;
		bool merged = false;
	public:
		Tile(uint8_t val, uint16_t tID);
		Tile();
		bool canMerge(Tile tile);
		uint8_t getVal();
		uint16_t getID();
		bool hasMoved();
		bool hasMerged();
		bool madeFirstMove();
		void doubleVal(int i, int j);
		void updatePosition(int i, int j);
		void updateOldPosition(int i, int j);
		void getValues(int* i, int* j, int* val);
		void getOldValues(int* i, int* j, int* val);
		void resetMergeStatus(); // Used to avoid tiles merging twice in the same move.
		void resetMoved();
		void resetMerge();

	};


	class Board {
	private:
		std::vector < std::vector<Tile*> > boardVec = {
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr} };
		std::vector<std::vector<Tile*>> undoBoardVec = {
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr} };
		std::vector<std::vector<Tile*>> redoBoardVec = {
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr} };
		uint32_t score = 0;
		uint8_t vCap = 0;			 // Board capacity
		int8_t tilesChangedUndo = 0; // To be able to update board capacity after undoing
		int8_t tilesChangedRedo = 0; // To be able to update board capacity after redoing
		int8_t ScoreChangedUndo = 0; // To be able to update board capacity after undoing
		int8_t ScoreChangedRedo = 0; // To be able to update board capacity after redoing
		bool onUndo = false;		 // If current board is undo board (can't undo twice in a row)
		bool onRedo = false;		 // If current board is redo board (can't undo twice in a row)
		bool madeFirstMove = false;  // Used so user can't undo the initial board creation
		
		std::vector<std::vector<Tile*>> copyBoard(std::vector<std::vector<Tile*>>&);
		void resetMergeStatus();     // Resets all tiles' haveMoved flag
		void resetMovedStatus();     // Resets all tiles' haveMoved flag
		void addNewTile();
		bool makeMoveH(Direction d);
		bool makeMoveV(Direction d);
		bool canCreateTile();
		bool canMergeRemaining();
	public:
		Board();
		~Board();
		std::vector<std::vector<Tile*>>& getBoard();
		int32_t getScore();
		bool isGameComplete();
		bool makeMove(Direction d);
		void undoMove();
		void redoMove();
	};

};

