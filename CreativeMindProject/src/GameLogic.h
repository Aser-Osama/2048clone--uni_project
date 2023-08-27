#pragma once
#include <string>
#include <vector>
namespace GameLogic
{
	enum class Direction {
		none, up, down, left, right
	};

	class Tile {
	private:
		uint8_t tileVal;
		uint8_t oldTileVal = -1;
		static uint16_t tileID;
		uint8_t oldTile_I = -1;
		uint8_t oldTile_J = -1;
		bool merged = false;
	public:
		Tile(uint8_t val, uint16_t tID);
		Tile();
		//Tile(Tile& t);
		uint8_t getVal();
		bool canMerge(Tile tile);
		void doubleVal(int i, int j);
		void updateOldPosition(int i, int j);
		void getOldValues(int* i, int* j, int* val);
		void resetMergeStatus();
		uint16_t getID();
	};


	class Board {
	private:
		std::vector<std::vector<Tile*>> boardVec = {
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
		uint8_t vCap = 0;			 // Board capacity
		int8_t tilesChangedUndo = 0; // To be able to update board capacity after undoing
		int8_t tilesChangedRedo = 0; // To be able to update board capacity after redoing
		int8_t ScoreChangedUndo = 0; // To be able to update board capacity after undoing
		int8_t ScoreChangedRedo = 0; // To be able to update board capacity after redoing
		bool onUndo = false;		 // If current board is undo board (can't undo twice in a row)
		bool onRedo = false;		 // If current board is redo board (can't undo twice in a row)
		bool madeFirstMove = false;  // Used so user can't undo the initial board creation
		uint32_t score = 0;
		bool makeMoveH(Direction d);
		bool makeMoveV(Direction d);
		void resetMergeStatus();
		void addNewTile();
		bool canCreateTile();
		bool canMergeRemaining();
		std::vector<std::vector<Tile*>> copyBoard(std::vector<std::vector<Tile*>>&);
	public:
		~Board();
		Board();
		std::vector<std::vector<Tile*>>& getBoard();
		void makeMove(Direction d);
		bool isGameComplete();
		void undoMove();
		void redoMove();
		int32_t getScore() { return score; }
	};

};

