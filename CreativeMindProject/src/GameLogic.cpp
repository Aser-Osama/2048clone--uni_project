#include "GameLogic.h"
#include <iostream>
#include <string>
#include <random>
#include <vector>
#pragma once

namespace GameLogic {
	Tile::Tile(uint8_t pos) {
		gridPos = pos;
		int rand = std::rand() % 11;
		if (rand == 10) {
			tileVal = 2;
		}
		else {
			tileVal = 1;
		}
	}
	void Tile::doubleVal() {
		this->tileVal = this->tileVal + 1;
		merged = true;
	}
	uint8_t Tile::getVal() {
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
		std::srand(std::time(0));
		for (uint8_t i = 0; i < 2; i++) {
			uint8_t randRow, randCol;
			do {
				randRow = std::rand() % 4;
				randCol = std::rand() % 4;
			} while (this->boardVec[randRow][randCol] != nullptr);
			this->boardVec[randRow][randCol] = new Tile(randRow * 4 + randCol);
			vCap++;
		}
	}

	void Board::resetMergeStatus() {
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 4; j++) {
				if (boardVec[i][j]) {
					boardVec[i][j]->resetMergeStatus();
				}
			}
		}
	}

	bool Board::canMergeRemaining()
	{
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 4; j++) {
				if (j < 3
					&& boardVec[i][j] != nullptr
					&& boardVec[i][j + 1] != nullptr
					&& boardVec[i][j]->canMerge(*boardVec[i][j + 1])) {
					return true;
				}
				if (i < 3
					&& boardVec[i][j] != nullptr
					&& boardVec[i + 1][j] != nullptr
					&& boardVec[i][j]->canMerge(*boardVec[i + 1][j])) {
					return true;
				}
			}
		}
		return false;
	}

	void Board::makeMove(char move) {
		if (GeneralMoveLogic(move)){
			this->addNewTile();
			this->resetMergeStatus();
		}
	}



	void Board::printv() {
		for (uint8_t i = 0; i < boardVec.size(); i++) {
			for (uint8_t j = 0; j < boardVec.size(); j++) {
				if (boardVec[i][j])
					std::cout << boardVec[i][j]->getVal();
				else
					std::cout << 0;
				std::cout << "\t";
			}
			std::cout << std::endl;
		}
	}

	void Board::addNewTile()
	{
		if (canCreateTile()) {
			vCap++;
			uint8_t randRow, randCol;
			do {
				randRow = std::rand() % 4;
				randCol = std::rand() % 4;
			} while (this->boardVec[randRow][randCol] != nullptr);
			this->boardVec[randRow][randCol] = new Tile(randRow * 4 + randCol);
		}

	}

	std::vector<std::vector<Tile*>>& Board::getBoard()
	{
		return boardVec;
	}

	bool Board::canCreateTile() {
			// for (std::vector<Tile*> v : boardVec) {
			// 		for (Tile* t : v) {
			// 			if (t == nullptr) {
			// 				return true;
			// 			}
			// 	}
			// }
		return !(vCap==16);  // No empty spot found
	}

	bool Board::isGameComplete() {
		return !(canCreateTile() || canMergeRemaining());
	}


	bool Board::GeneralMoveLogic(char dir)
	{
		bool vertical = (dir == 'u' || dir == 'd');
		bool moved = false;
		if (vertical) {
			int dI = (dir == 'u') ? -1 : 1;
			bool up = (dir == 'u');
			for (int i = up ? 1 : 2; up ? i < 4 : i >= 0; i += -(dI)) {
				for (int j = 0; j < 4; j++) {
					if (boardVec[i][j] == nullptr)
						continue;
					if (boardVec[i + dI][j] == nullptr) { //above tile is empty
						boardVec[i + dI][j] = boardVec[i][j];
						boardVec[i][j] = nullptr;
						moved = true;
						for (uint8_t k = i + dI; up ? k > 0 : k < 3; k += dI) { //to move up as far as possible
							if (boardVec[k][j] == nullptr)
								continue;

							if (boardVec[k + dI][j] == nullptr) { //above tile is also empty
								boardVec[k + dI][j] = boardVec[k][j];
								boardVec[k][j] = nullptr;
							}
							else if (boardVec[k][j]->canMerge(*boardVec[k + dI][j])) { //above tile is mergable
								boardVec[k + dI][j]->doubleVal();
								delete boardVec[i][j];
								boardVec[k][j] = nullptr;
								vCap--;
							}
						}
					}

					//////////

					else if (boardVec[i][j]->canMerge(*boardVec[i + dI][j])) { //above tile is mergable
						moved = true;
						boardVec[i + dI][j]->doubleVal();
						delete boardVec[i][j];
						boardVec[i][j] = nullptr;
						vCap--;
						for (uint8_t k = i + dI; up ? k > 0 : k < 3; k += dI) { //to move up as far as possible
							if (boardVec[k][j] == nullptr)
								continue;

							if (boardVec[k + dI][j] == nullptr) { //above tile is empty
								boardVec[k + dI][j] = boardVec[k][j];
								boardVec[k][j] = nullptr;
							}
							else if (boardVec[k][j]->canMerge(*boardVec[k + dI][j])) { //above tile is also mergable
								boardVec[k + dI][j]->doubleVal();
								delete boardVec[k][j];
								boardVec[k][j] = nullptr;
								vCap--;
							}
						}
					}
				}
			}
		}
		else {
			int dJ = (dir == 'l') ? -1 : 1;
			bool Left = (dir == 'l');
			for (int8_t i = 0; i < 4; i++) {
				for (int8_t j = Left ? 1 : 2; Left ? j < 4 : j >= 0; j += (-dJ)) {
					if (boardVec[i][j] == nullptr)
						continue;

					if (boardVec[i][j + dJ] == nullptr) { //left tile is empty
						boardVec[i][j + dJ] = boardVec[i][j];
						boardVec[i][j] = nullptr;
						moved = true;
						for (int8_t k = j + dJ; Left ? k > 0: k < 3; k += dJ) { //to move left as far as possible
							if (boardVec[i][k] == nullptr)
								continue;

							if (boardVec[i][k + dJ] == nullptr) { //left tile is also empty
								boardVec[i][k + dJ] = boardVec[i][k];
								boardVec[i][k] = nullptr;
							}
							else if (boardVec[i][k]->canMerge(*boardVec[i][k + dJ])) { //left tile is mergable
								boardVec[i][k + dJ]->doubleVal();
								vCap--;
								delete boardVec[i][k];
								boardVec[i][k] = nullptr;
							}
						}
					}

					//////////

					else if (boardVec[i][j]->canMerge(*boardVec[i][j + dJ])) { //above tile is mergable
						moved = true;
						boardVec[i][j + dJ]->doubleVal();
						delete boardVec[i][j];
						boardVec[i][j] = nullptr;
						vCap--;
						for (int8_t k = j + dJ; Left ? k > 0: k < 3; k += dJ) {
							if (boardVec[k][j] == nullptr)
								continue;

							if (boardVec[i][k + dJ] == nullptr) { //above tile is empty
								boardVec[i][k + dJ] = boardVec[i][k];
								boardVec[i][k] = nullptr;
							}
							else if (boardVec[i][k]->canMerge(*boardVec[i][k + dJ])) { //above tile is also mergable
								boardVec[i][k]->doubleVal();
								delete boardVec[i][k];
								boardVec[i][k] = nullptr;
								vCap--;
							}
						}
					}
				}
			}
		}

		return moved;
	}
};