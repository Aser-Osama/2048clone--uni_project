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
		bool merged = false;
	public:
		Tile(uint8_t val);
		Tile();
		//Tile(Tile& t);
		uint8_t getVal();
		bool canMerge(Tile tile);
		void doubleVal();
		void resetMergeStatus();
	};


	class Board {
	private:
		std::vector<std::vector<Tile*>> boardVec =	{ 
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr}};
		std::vector<std::vector<Tile*>> undoBoardVec = {
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr}};
		std::vector<std::vector<Tile*>> redoBoardVec = {
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr},
								{nullptr, nullptr, nullptr, nullptr}};

		uint8_t vCap = 0;			  // Board capacity
		uint8_t tilesChangedUndo = 0; // To be able to update board capacity after undoing
		uint8_t tilesChangedRedo = 0; // To be able to update board capacity after redoing
		bool onUndo = false;		  // If current board is undo board (can't undo twice in a row)
		bool madeFirstMove = false;   // Used so user can't undo the initial board creation
	public:
		Board();
		bool isGameComplete();
		bool makeMoveH(Direction d);
		bool makeMoveV(Direction d);
		void makeMove(Direction d);
		std::vector<std::vector<Tile*>> saveOldBoard();
		void resetMergeStatus();
		bool canMergeRemaining();
		void printv(); //used for debugging along with gui
		void addNewTile();
		void undoMove();
		bool canCreateTile();
		std::vector<std::vector<Tile*>>& getBoard();

	};

};

