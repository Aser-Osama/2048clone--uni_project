#pragma once
#include <string>
#include <vector>
namespace GameLogic
{
	class Tile {
	private:
		int tileVal;
		std::string tilePath;
	public:
		Tile(int num); //Generate a given sized tile
		Tile(); //Generate a random tile (2 or 4)
		int getVal();
		bool canMerge(Tile tile);
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
		bool isMoveLegal(char move);
	};


};

