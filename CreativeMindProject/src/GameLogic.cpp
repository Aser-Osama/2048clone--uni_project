#include "GameLogic.h"
#include <iostream>
#include <string>
#include <random>
#include <vector>
#pragma once

namespace GameLogic {



	//Tile class definitions
	Tile::Tile(int num) {
		tileVal = num;
		tilePath = "CreativeMindProject\\Imgs\\" + std::to_string(num) + ".bmp";
	}
	Tile::Tile() {
		int rand = std::rand() % 11;
		if (rand == 10) {
			tileVal = 4;
			tilePath = "CreativeMindProject\\Imgs\\" + std::to_string(4) + ".bmp";
		}
		else {
			tileVal = 2;
			tilePath = "CreativeMindProject\\Imgs\\" + std::to_string(2) + ".bmp";
		}
	}
	int Tile::getVal() {
		return this->tileVal;
	}
	bool Tile::canMerge(Tile t) {
		return (t.tileVal == this->tileVal);
	}

	//Board class definitions
	Board::Board() {
		std::srand(std::time(0));
		for (int i = 0; i < 2; i++) {
			int randRow, randCol;
			do {
				randRow = std::rand() % 4;
				randCol = std::rand() % 4;
			} while (this->boardVec[randRow][randCol] != nullptr);
			this->boardVec[randRow][randCol] = new Tile();
		}
	}
	bool Board::isMoveLegal(char move) {
		bool isLegal = false;

			switch (move)
			{
			case 'u':
				for (int i = 1; i < 4; i++) {
					for (int j = 0; j < 4; j++) {
						if (boardVec[i][j] != nullptr && (boardVec[i - 1][j] == nullptr || boardVec[i][j]->canMerge(*boardVec[i - 1][j]))) {
							isLegal = true;
						}
					}
				}

				break;
			case 'd':

				break;
			case 'l':

				break;
			case 'r':

				break;
			default:
				break;
			}
		}

	}
	bool Board::isGameComplete() {
		//checks possible spots
		for (std::vector<Tile*> v : boardVec) {
			for (Tile* t : v) {
				if (t != nullptr) {
					return false;
				}
			}
		}

		return true;
	}

}

