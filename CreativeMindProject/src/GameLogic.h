#pragma once
#include <string>
#include <vector>
namespace GameLogic
{
	class Tile {
	private:
		uint8_t tileVal;
		std::string tilePath;
		bool merged = false;
		uint8_t gridPos;
	public:
		Tile(uint8_t size, uint8_t pos); //Generate a given sized tile
		Tile(uint8_t pos); //Generate a random tile (2 or 4)
		uint8_t getVal();
		bool canMerge(Tile tile);
		void doubleVal();
		void resetMergeStatus();
		void setPos(uint8_t, uint8_t);
	};

	class Board {
	private:
		std::vector<std::vector<Tile*>> boardVec =	{ 
												{nullptr, nullptr, nullptr, nullptr},
												{nullptr, nullptr, nullptr, nullptr},
												{nullptr, nullptr, nullptr, nullptr},
												{nullptr, nullptr, nullptr, nullptr}};

	public:
		Board();
		bool isGameComplete();
		bool makeMove(char move);
		void resetMergeStatus();
		bool canMergeRemaining();
		void printv();
		void addNewTile();
		bool canCreateTile();
		std::vector<std::vector<Tile*>>* getBoard();
		
		//logic for moving is split into seprate functions for ease of reading
		bool moveUpLogic();
		bool moveDownLogic();
		bool moveRightLogic();
		bool moveLeftLogic();
	};


};

