#include "GameLogic.h"
#include <math.h>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#pragma once

namespace GameLogic {
	Tile::Tile(uint8_t val, uint16_t tID) {
		tileVal = val;
		tileID = tID;
	}
	Tile::Tile() {
		tileID++;
		int8_t rand = std::rand() % 11;
		if (rand == 10) {
			tileVal = 2;
		}
		else {
			tileVal = 1;
		}
	}
	void Tile::doubleVal(int i, int j) {
		this->Position.justMerged = true;
		oldTileVal = tileVal;
		this->tileVal = this->tileVal + 1;
		merged = true;
	}
	void Tile::updateOldPosition(int i, int j) {
		Position.justMoved = true;
		Position.madeFirstMove = true;
		this->Position.oldTile_I = i;
		this->Position.oldTile_J = j;
	}
	void Tile::updatePosition(int i, int j) {
		this->Position.Tile_I = i;
		this->Position.Tile_J = j;
	}
	void Tile::getOldValues(int* i, int* j, int* val) {
		*i = this->Position.oldTile_I;
		*j = this->Position.oldTile_J;
		*val = this->oldTileVal;
	}
	void Tile::getValues(int* i, int* j, int* val) {
		*i = this->Position.Tile_I;
		*j = this->Position.Tile_J;
		*val = this->tileVal;
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
	uint16_t Tile::getID() {
		return tileID;
	}
	bool Tile::hasMoved() {
		return Position.justMoved;
	}
	bool Tile::hasMerged()
	{
		return Position.justMerged;
	}
	bool Tile::madeFirstMove()
	{
		return Position.madeFirstMove;
	}
	void Tile::resetMoved() {
		Position.justMoved = false;
	}
	void Tile::resetMerge() {
		this->Position.justMerged = false;
	}
	uint16_t Tile::tileID = 0;

	// Board class definitions
	Board::Board() {
		std::srand(std::time(0));
		for (uint8_t i = 0; i < 2; i++) {
			uint8_t randRow, randCol;
			do {
				randRow = std::rand() % 4;
				randCol = std::rand() % 4;
			} while (this->boardVec[randRow][randCol] != nullptr);
			this->boardVec[randRow][randCol] = new Tile();
			this->boardVec[randRow][randCol]->updatePosition(randRow, randCol);
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
	void Board::resetMovedStatus() {
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 4; j++) {
				if (boardVec[i][j]) {
					boardVec[i][j]->resetMoved();
					boardVec[i][j]->resetMerge();
				}
			}
		}
	}

	bool Board::canMergeRemaining() {
		for (uint8_t i = 0; i < 4; i++) {
			for (uint8_t j = 0; j < 4; j++) {
				if (j < 3 && boardVec[i][j] != nullptr && boardVec[i][j + 1] != nullptr &&
					boardVec[i][j]->canMerge(*boardVec[i][j + 1])) {
					return true;
				}
				if (i < 3 && boardVec[i][j] != nullptr && boardVec[i + 1][j] != nullptr &&
					boardVec[i][j]->canMerge(*boardVec[i + 1][j])) {
					return true;
				}
			}
		}
		return false;
	}

	bool Board::makeMoveV(Direction dir) {
		int8_t dI = (dir == Direction::up) ? -1 : 1;
		bool up = (dir == Direction::up);
		bool moved = false;
		for (int8_t i = up ? 1 : 2; up ? i < 4 : i >= 0; i += -(dI)) {
			for (int8_t j = 0; j < 4; j++) {
				if (boardVec[i][j] == nullptr)
					continue;

				boardVec[i][j]->resetMoved();
				// The tile can be moved up or down
				if (boardVec[i + dI][j] == nullptr) {
					boardVec[i + dI][j] = boardVec[i][j];
					boardVec[i][j]->updateOldPosition(i, j);
					boardVec[i][j]->updatePosition(i + dI, j);
					boardVec[i][j] = nullptr;
					moved = true;

					// Checking if tile is not stuck in place
					for (uint8_t k = i + dI; up ? k > 0 : k < 3; k += dI) {
						if (boardVec[k][j] == nullptr)
							continue;

						// The tile can be moved up or down
						if (boardVec[k + dI][j] == nullptr) {
							boardVec[k + dI][j] = boardVec[k][j];
							//boardVec[k][j]->updateOldPosition(k, j);
							boardVec[k][j]->updatePosition(k + dI, j);
							boardVec[k][j] = nullptr;
						}

						// The tile can be merged with one above or below
						else if (boardVec[k][j]->canMerge(*boardVec[k + dI][j])) {
							boardVec[k + dI][j]->doubleVal(i, j);
							boardVec[k + dI][j]->updatePosition(k + dI, j);
							score += static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[k + dI][j]->getVal())));
							this->ScoreChangedUndo -= static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[k + dI][j]->getVal())));
							delete boardVec[i][j];
							boardVec[k][j] = nullptr;
							this->vCap--;
							this->tilesChangedUndo++;
						}
					}
				}

				// The tile can be merged with one above or below
				else if (boardVec[i][j]->canMerge(*boardVec[i + dI][j])) {  // above tile is mergable
					moved = true;
					boardVec[i + dI][j]->doubleVal(i, j);
					boardVec[i + dI][j]->updateOldPosition(i, j);
					boardVec[i + dI][j]->updatePosition(i + dI, j);
					score += static_cast<uint32_t>(
						std::pow(2, static_cast<uint32_t>(boardVec[i + dI][j]->getVal())));
					this->ScoreChangedUndo -= static_cast<uint32_t>(
						std::pow(2, static_cast<uint32_t>(boardVec[i + dI][j]->getVal())));
					delete boardVec[i][j];
					boardVec[i][j] = nullptr;
					this->vCap--;
					this->tilesChangedUndo++;
					for (uint8_t k = i + dI; up ? k > 0 : k < 3;
						k += dI) {  // to move up as far as possible
						if (boardVec[k][j] == nullptr)
							continue;

						// The tile can be moved up or down
						if (boardVec[k + dI][j] == nullptr) {  // above tile is empty
							boardVec[k + dI][j] = boardVec[k][j];
							//boardVec[k + dI][j]->updateOldPosition(i, j);
							boardVec[k][j]->updatePosition(k + dI, j);
							boardVec[k][j] = nullptr;
						}

						// The tile can be merged with one above or below
						else if (boardVec[k][j]->canMerge(
							*boardVec[k + dI][j])) {  // above tile is also mergable
							boardVec[k + dI][j]->doubleVal(i, j);
							boardVec[k + dI][j]->updatePosition(k + dI, j);
							score += static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[k + dI][j]->getVal())));
							this->ScoreChangedUndo -= static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[k + dI][j]->getVal())));
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
		int8_t dJ = (dir == Direction::left) ? -1 : 1;
		bool Left = (dir == Direction::left);
		bool moved = false;
		for (int8_t i = 0; i < 4; i++) {
			for (int8_t j = Left ? 1 : 2; Left ? j < 4 : j >= 0; j += (-dJ)) {
				if (boardVec[i][j] == nullptr)
					continue;

				boardVec[i][j]->resetMoved();
				// Tile can be moved right or left
				if (boardVec[i][j + dJ] == nullptr) {
					boardVec[i][j + dJ] = boardVec[i][j];
					boardVec[i][j]->updateOldPosition(i, j);
					boardVec[i][j]->updatePosition(i, j + dJ);
					boardVec[i][j] = nullptr;
					moved = true;

					for (int8_t k = j + dJ; Left ? k > 0 : k < 3;
						k += dJ) {  // to move left as far as possible
						if (boardVec[i][k] == nullptr)
							continue;

						if (boardVec[i][k + dJ] == nullptr) {  // left tile is also empty
							boardVec[i][k + dJ] = boardVec[i][k];
							//boardVec[i][k]->updateOldPosition(i, k);
							boardVec[i][k]->updatePosition(i, k + dJ);

							boardVec[i][k] = nullptr;
						}
						else if (boardVec[i][k]->canMerge(
							*boardVec[i][k + dJ])) {  // left tile is mergable
							boardVec[i][k + dJ]->doubleVal(i, j);
							boardVec[i][k + dJ]->updatePosition(i, k + dJ);
							score += static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[i][k + dJ]->getVal())));
							ScoreChangedUndo -= static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[i][k + dJ]->getVal())));
							this->vCap--;
							this->tilesChangedUndo++;
							delete boardVec[i][k];
							boardVec[i][k] = nullptr;
						}
					}
				}

				//////////

				else if (boardVec[i][j]->canMerge(*boardVec[i][j + dJ])) {  // left tile is mergable
					moved = true;
					boardVec[i][j + dJ]->doubleVal(i, j);
					boardVec[i][j + dJ]->updatePosition(i, j + dJ);
					boardVec[i][j + dJ]->updateOldPosition(i, j);

					score += static_cast<uint32_t>(
						std::pow(2, static_cast<uint32_t>(boardVec[i][j + dJ]->getVal())));
					ScoreChangedUndo -= static_cast<uint32_t>(
						std::pow(2, static_cast<uint32_t>(boardVec[i][j + dJ]->getVal())));
					delete boardVec[i][j];
					boardVec[i][j] = nullptr;
					vCap--;
					this->tilesChangedUndo++;
					for (int8_t k = j + dJ; Left ? k > 0 : k < 3; k += dJ) {
						if (boardVec[k][j] == nullptr)
							continue;

						if (boardVec[i][k + dJ] == nullptr) {  // above tile is empty
							boardVec[i][k + dJ] = boardVec[i][k];
							//boardVec[i][k + dJ]->updateOldPosition(i, k);
							boardVec[i][k + dJ]->updatePosition(i, k + dJ);
							boardVec[i][k] = nullptr;
						}
						else if (boardVec[i][k]->canMerge(
							*boardVec[i][k + dJ])) {  // left tile is also mergable
							boardVec[i][k + dJ]->doubleVal(i, j);
							boardVec[i][k + dJ]->updatePosition(i, k + dJ);
							score += static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[i][k + dJ]->getVal())));
							ScoreChangedUndo -= static_cast<uint32_t>(std::pow(
								2, static_cast<uint32_t>(boardVec[i][k + dJ]->getVal())));
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

	std::vector<std::vector<Tile*>> Board::copyBoard(
		std::vector<std::vector<Tile*>>& b) {
		std::vector<std::vector<Tile*>> tmpBoard = {
			{nullptr, nullptr, nullptr, nullptr},
			{nullptr, nullptr, nullptr, nullptr},
			{nullptr, nullptr, nullptr, nullptr},
			{nullptr, nullptr, nullptr, nullptr} };
		for (int8_t i = 0; i < 4; i++) {
			for (int8_t j = 0; j < 4; j++) {
				if (b[i][j]) {
					Tile* newTile = new Tile(b[i][j]->getVal(), b[i][j]->getID());
					(tmpBoard)[i][j] = newTile;
				}
			}
		}
		return tmpBoard;
	}

	bool Board::makeMove(Direction d) {
		if (d == Direction::none)
			return true;
		
		this->resetMovedStatus();
		int8_t tmpChanged = tilesChangedUndo;
		tilesChangedUndo = 0;
		int8_t tmpScore = ScoreChangedUndo;
		ScoreChangedUndo = 0;

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
			tilesChangedUndo = tmpChanged;
			ScoreChangedUndo = tmpScore;
			this->resetMovedStatus();
		}
		return !move_made;
	}

	void Board::undoMove() {
		if (onUndo || !madeFirstMove)
			return;

		onUndo = true;
		onRedo = false;
		tilesChangedRedo = -tilesChangedUndo;
		vCap = vCap + tilesChangedUndo;
		tilesChangedUndo = 0;
		ScoreChangedRedo = -ScoreChangedUndo;
		score = score + ScoreChangedUndo;
		ScoreChangedUndo = 0;

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
		ScoreChangedUndo = -ScoreChangedRedo;
		score += ScoreChangedRedo;
		ScoreChangedRedo = 0;

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

	void Board::addNewTile() {
		if (canCreateTile()) {
			vCap++;
			this->tilesChangedUndo--;
			uint8_t randRow, randCol;
			do {
				randRow = std::rand() % 4;
				randCol = std::rand() % 4;
			} while (this->boardVec[randRow][randCol] != nullptr);
			this->boardVec[randRow][randCol] = new Tile();
			this->boardVec[randRow][randCol]->updatePosition(randRow, randCol);
		}
	}

	std::vector<std::vector<Tile*>>& Board::getBoard() {
		return boardVec;
	}

	int32_t Board::getScore() {
		return score;
	}

	bool Board::canCreateTile() {
		return !(vCap == 16);
	}

	bool Board::isGameComplete() {
		return !(canCreateTile() || canMergeRemaining());
	}

	Board::~Board() {
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

};  // namespace GameLogic