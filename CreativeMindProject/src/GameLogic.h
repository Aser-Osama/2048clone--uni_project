#pragma once
#include <string>
#include <vector>
namespace GameLogic
{
	class Tile {
	private:
		int tileVal;
		std::string tilePath;
		bool merged = false;
	public:
		Tile(int num); //Generate a given sized tile
		Tile(); //Generate a random tile (2 or 4)
		int getVal();
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

	public:
		Board();
		bool isGameComplete();
		bool makeMove(char move);
		void resetMergeStatus();
		bool canMergeRemaining();
		void printv();
		void addNewTile();

		
		//logic for moving is split into seprate functions for ease of reading
		bool moveUpLogic();
		bool moveDownLogic();
		bool moveRightLogic();
		bool moveLeftLogic();
	};


};

