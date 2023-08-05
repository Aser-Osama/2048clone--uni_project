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
	void Tile::doubleVal() {
		this->tileVal = this->tileVal * 2;
		this->tilePath = "CreativeMindProject\\Imgs\\" + std::to_string(this->tileVal) + ".bmp";
		merged = true;
	}
	int Tile::getVal() {
		return this->tileVal;
	}
	bool Tile::canMerge(Tile t) {
		return (t.tileVal == this->tileVal && !(merged || t.merged));
	}
	void Tile::resetMergeStatus() {
		merged = false;
	}


	//Board class definitions
	Board::Board() {
		// std::srand(std::time(0));
		// for (int i = 0; i < 2; i++) {
		// 	int randRow, randCol;
		// 	do {
		// 		randRow = std::rand() % 4;
		// 		randCol = std::rand() % 4;
		// 	} while (this->boardVec[randRow][randCol] != nullptr);
		// 	this->boardVec[randRow][randCol] = new Tile();
		// }
		Tile* t11 = new Tile(2);
		Tile* t22 = new Tile(2);
		Tile* t1 = new Tile(2);
		Tile* t2 = new Tile(2);
		Tile* t3 = new Tile(2);
		Tile* t44 = new Tile(2);
		Tile* t4 = new Tile(2);
		Tile* t5 = new Tile(2);
		boardVec[0][0] = t11;
		boardVec[0][1] = t22;
		boardVec[0][2] = t1;
		boardVec[0][3] = t2;
		boardVec[1][2] = t3;
		boardVec[2][1] = t4;
		boardVec[3][0] = t5;
		boardVec[3][3] = t44;
	}

	void Board::resetMergeStatus() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (boardVec[i][j]) {
					boardVec[i][j]->resetMergeStatus();
				}
			}
		}
	}

	bool Board::makeMove(char move) {
		this->resetMergeStatus();
		switch (move) {
		case 'u': //checks if i can push all rows up
			return moveUpLogic();
			break;
		case 'd': //checks if i can push all columns down
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 4; j++) {
					if (boardVec[i][j] != nullptr && (boardVec[i + 1][j] == nullptr || boardVec[i][j]->canMerge(*boardVec[i + 1][j]))) {
						return true;
					}
				}
			}
			break;

		case 'l':
			return moveLeftLogic();
			break;
		case 'r':

			break;
		default:
			break;
		}
	}



	void Board::printv() {
		for (int i = 0; i < boardVec.size(); i++) {
			for (int j = 0; j < boardVec.size(); j++) {
				if (boardVec[i][j])
					std::cout << boardVec[i][j]->getVal();
				else
					std::cout << 0;
			}
			std::cout << std::endl;
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

	bool Board::moveUpLogic()
	{
		bool moved = false;
		for (int i = 1; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (boardVec[i][j] == nullptr)
					continue;

				if (boardVec[i - 1][j] == nullptr) { //above tile is empty
					boardVec[i - 1][j] = boardVec[i][j];
					boardVec[i][j] = nullptr;
					moved = true;
					for (int k = i - 1; k > 0; k--) { //to move up as far as possible
						if (boardVec[k][j] == nullptr)
							continue;

						if (boardVec[k - 1][j] == nullptr) { //above tile is also empty
							boardVec[k - 1][j] = boardVec[k][j];
							boardVec[k][j] = nullptr;
						}
						else if (boardVec[k][j]->canMerge(*boardVec[k - 1][j])) { //above tile is mergable
							boardVec[k - 1][j]->doubleVal();
							delete boardVec[i][j];
							boardVec[k][j] = nullptr;
						}
					}
				}

				//////////

				else if (boardVec[i][j]->canMerge(*boardVec[i - 1][j])) { //above tile is mergable
					moved = true;
					boardVec[i - 1][j]->doubleVal();
					delete boardVec[i][j];
					boardVec[i][j] = nullptr;
					for (int k = i - 1; k > 0; k--) {
						if (boardVec[k][j] == nullptr)
							continue;

						if (boardVec[k - 1][j] == nullptr) { //above tile is empty
							boardVec[k - 1][j] = boardVec[k][j];
							boardVec[k][j] = nullptr;
						}
						else if (boardVec[k][j]->canMerge(*boardVec[k - 1][j])) { //above tile is also mergable
							boardVec[k - 1][j]->doubleVal();
							delete boardVec[k][j];
							boardVec[k][j] = nullptr;
						}
					}
				}
			}
		}
		return moved;
	}

	bool Board::moveLeftLogic()
	{
		bool moved = false;
		for (int i = 0; i < 4; i++) {
			for (int j = 1; j < 4; j++) {
				if (boardVec[i][j] == nullptr)
					continue;

				if (boardVec[i][j-1] == nullptr) { //left tile is empty
					boardVec[i][j-1] = boardVec[i][j];
					boardVec[i][j] = nullptr;
					moved = true;
					for (int k = j - 1; k > 0; k--) { //to move left as far as possible
						if (boardVec[i][k] == nullptr)
							continue;

						if (boardVec[i][k-1] == nullptr) { //left tile is also empty
							boardVec[i][k-1] = boardVec[i][k];
							boardVec[i][k] = nullptr;
						}
						else if (boardVec[i][k]->canMerge(*boardVec[i][k-1])) { //left tile is mergable
							boardVec[i][k-1]->doubleVal();
							delete boardVec[i][k];
							boardVec[i][k] = nullptr;
						}
					}
				}

				//////////

				else if (boardVec[i][j]->canMerge(*boardVec[i][j-1])) { //above tile is mergable
					moved = true;
					boardVec[i][j-1]->doubleVal();
					delete boardVec[i][j];
					boardVec[i][j] = nullptr;
					for (int k = j - 1; k > 0; k--) {
						if (boardVec[k][j] == nullptr)
							continue;

						if (boardVec[i][k-1] == nullptr) { //above tile is empty
							boardVec[i][k-1] = boardVec[i][k];
							boardVec[i][k] = nullptr;
						}
						else if (boardVec[i][k]->canMerge(*boardVec[i][k-1])) { //above tile is also mergable
							boardVec[i][k]->doubleVal();
							delete boardVec[i][k];
							boardVec[i][k] = nullptr;
						}
					}
				}
			}
		}
		return moved;
	}

}

