#pragma once
#include <string>
#include <vector>
namespace GameLogic
{
	class Tile {
	private:
		uint8_t tileVal;
		bool merged = false;
		uint8_t gridPos;
	public:
		Tile(uint8_t pos); //Generate a random tile (2 or 4)
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
		uint8_t vCap = 0;
	public:
		Board();
		bool isGameComplete();
		void makeMove(char move);
		void resetMergeStatus();
		bool canMergeRemaining();
		void printv(); //used for debugging along with gui
		void addNewTile();
		bool canCreateTile();
		std::vector<std::vector<Tile*>>* getBoard();
		bool GeneralMoveLogic(char);
	};


};

