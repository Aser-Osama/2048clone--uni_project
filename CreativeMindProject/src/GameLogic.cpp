#include "GameLogic.h"
#include <iostream>
#include <string>
#include <random>
#include <math.h>
#include <vector>
#pragma once

namespace GameLogic {
	Tile::Tile(uint8_t val)
	{
		tileVal = val;
	}
	Tile::Tile() {
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
			this->boardVec[randRow][randCol] = new Tile();
			this->vCap++;
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

	bool Board::makeMoveV(Direction dir){
		int dI = (dir == Direction::up) ? -1 : 1;
		bool up = (dir == Direction::up);
		bool moved = false;
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
							score += unsigned(std::pow(2,unsigned(boardVec[k + dI][j]->getVal())));
							delete boardVec[i][j];
							boardVec[k][j] = nullptr;
							this->vCap--;
							this->tilesChangedUndo++;
						}
					}
				}
				else if (boardVec[i][j]->canMerge(*boardVec[i + dI][j])) { //above tile is mergable
					moved = true;
					boardVec[i + dI][j]->doubleVal();
					score += unsigned(std::pow(2,unsigned(boardVec[i + dI][j]->getVal())));
					delete boardVec[i][j];
					boardVec[i][j] = nullptr;
					this->vCap--;
					this->tilesChangedUndo++;
					for (uint8_t k = i + dI; up ? k > 0 : k < 3; k += dI) { //to move up as far as possible
						if (boardVec[k][j] == nullptr)
							continue;

						if (boardVec[k + dI][j] == nullptr) { //above tile is empty
							boardVec[k + dI][j] = boardVec[k][j];
							boardVec[k][j] = nullptr;
						}
						else if (boardVec[k][j]->canMerge(*boardVec[k + dI][j])) { //above tile is also mergable
							boardVec[k + dI][j]->doubleVal();
							score += unsigned(std::pow(2,unsigned(boardVec[k + dI][j]->getVal())));
							delete boardVec[k][j];
							boardVec[k][j] = nullptr;
							vCap--;
							this->tilesChangedUndo++;
						}
					}
				}
			}
		}
		return moved;
	}
	
	bool Board::makeMoveH(Direction dir) {
		int dJ = (dir == Direction::left) ? -1 : 1;
		bool Left = (dir == Direction::left);
		bool moved = false;
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
							score += unsigned(std::pow(2,unsigned(boardVec[i][k + dJ]->getVal())));
							this->vCap--;
							this->tilesChangedUndo++;
							delete boardVec[i][k];
							boardVec[i][k] = nullptr;
						}
					}
				}

				//////////

				else if (boardVec[i][j]->canMerge(*boardVec[i][j + dJ])) { //left tile is mergable
					moved = true;
					boardVec[i][j + dJ]->doubleVal();
					score += unsigned(std::pow(2,unsigned(boardVec[i][j + dJ]->getVal())));
					delete boardVec[i][j];
					boardVec[i][j] = nullptr;
					vCap--; 
					this->tilesChangedUndo++;
					for (int8_t k = j + dJ; Left ? k > 0: k < 3; k += dJ) {
						if (boardVec[k][j] == nullptr)
							continue;

						if (boardVec[i][k + dJ] == nullptr) { //above tile is empty
							boardVec[i][k + dJ] = boardVec[i][k];
							boardVec[i][k] = nullptr;
						}
						else if (boardVec[i][k]->canMerge(*boardVec[i][k + dJ])) { //left tile is also mergable
							boardVec[i][k]->doubleVal();
							score += unsigned(std::pow(2,unsigned(boardVec[i][k + dJ]->getVal())));
							delete boardVec[i][k];
							boardVec[i][k] = nullptr;
							vCap--;
							this->tilesChangedUndo++;
						}
					}
				}
			}
		}
		return moved;
	}

	std::vector<std::vector<Tile*>> Board::copyBoard(std::vector<std::vector<Tile*>>& b)
	{
		std::vector<std::vector<Tile*>> tmpBoard = 
												{
												{nullptr, nullptr, nullptr, nullptr},
												{nullptr, nullptr, nullptr, nullptr},
												{nullptr, nullptr, nullptr, nullptr},
												{nullptr, nullptr, nullptr, nullptr}};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (b[i][j]) {
					Tile* newTile = new Tile(b[i][j]->getVal());
					(tmpBoard)[i][j] = newTile;
				}
			}
		}
		return tmpBoard;
	}

	void Board::makeMove(Direction d) {
		if (d == Direction::none)
			return;
		
		int tmp = tilesChangedUndo;			
		tilesChangedUndo = 0;

		std::vector<std::vector<Tile*>> tmpBoard = this->copyBoard(boardVec);
		bool vertical = (d == Direction::up || d == Direction::down);
		bool move_made = false;
		if (vertical)
			move_made = makeMoveV(d);
		else
			move_made = makeMoveH(d);

		if (move_made) {
			madeFirstMove = true;
			this->addNewTile();
			this->resetMergeStatus();
			this->undoBoardVec = tmpBoard;
			onUndo = false;
		}
		else {
			tilesChangedUndo = tmp;
		}
	}

	void Board::undoMove() {
		if (onUndo || !madeFirstMove)
			return;
		onUndo = true;
		onRedo = false;
		tilesChangedRedo = -tilesChangedUndo;
		vCap = vCap + tilesChangedUndo;
		tilesChangedUndo = 0;


		for (auto& r : redoBoardVec) {
			for (auto& t : r) {
				if (t) {
					delete t;
					t = nullptr;
				}
			}
		}

		redoBoardVec = copyBoard(boardVec);
		boardVec = copyBoard(undoBoardVec);
	}

	void Board::redoMove() {
		if (!onUndo || !madeFirstMove || onRedo)
			return;
		onUndo = false;
		onRedo = true;
		tilesChangedUndo = -tilesChangedRedo;
		vCap = vCap + tilesChangedRedo;
		tilesChangedRedo = 0;

		for (auto& r : undoBoardVec) {
			for (auto& t : r) {
				if (t) {
					delete t;
					t = nullptr;
				}
			}
		}
		undoBoardVec = copyBoard(boardVec);
		boardVec = copyBoard(redoBoardVec);
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
			this->tilesChangedUndo--;
			uint8_t randRow, randCol;
			do {
				randRow = std::rand() % 4;
				randCol = std::rand() % 4;
			} while (this->boardVec[randRow][randCol] != nullptr);
			this->boardVec[randRow][randCol] = new Tile();
		}

	}

	std::vector<std::vector<Tile*>>& Board::getBoard()
	{
		return boardVec;
	}

	bool Board::canCreateTile() {
		return !(vCap==16);  // No empty spot found
	}

	bool Board::isGameComplete() {
		return !(canCreateTile() || canMergeRemaining());
	}
	Board::~Board()
	{
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 4; j++) {
				if (boardVec[i][j]) {
					delete boardVec[i][j];
					boardVec[i][j] = nullptr;
				}
				if (undoBoardVec[i][j]) {
					delete undoBoardVec[i][j];
					undoBoardVec[i][j] = nullptr;
				}				
				if (redoBoardVec[i][j]) {
					delete redoBoardVec[i][j];
					redoBoardVec[i][j] = nullptr;
				}
			}
		}
	}

};