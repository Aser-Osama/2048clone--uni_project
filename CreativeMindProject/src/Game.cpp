#include "Game.h"
#include <SDL.h>
#include <Windows.h>
#include <iostream>
#include "GameLogic.h"
#include <vector>
#include <map>
namespace Game { 
    bool ButtonBounds::withinBounds(int16_t x, int16_t y)
    {
        return (x > this->startX && x < this->endX
            && y > this->startY && y < this->endY);
    }

    void Game::restartGame()
    {
        delete board;
        board = new GameLogic::Board;

    }

    Game::Game() : gGUI("2048 CLONE", 1280, 720)
    {
        if (SDL_Init(SDL_INIT_VIDEO) > 0) {
            std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
            exit(1);
        }
        if (TTF_Init() < 0) {
            std::cout << "SDL_TTF failed" << SDL_GetError() << std::endl;
            exit(1);
        }
        gGUI.loadTextures();
        board = new GameLogic::Board();
        gGUI.renderScene(board->getBoard(), board->getScore());
    }

    void Game::run() {
        bool gameRunning = true;
        SDL_Event event;
        while (gameRunning) {
            uint64_t start = SDL_GetPerformanceCounter();
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || board->isGameComplete()) {
                    gameRunning = false;
                    break;
                }
                else if (event.type == SDL_KEYDOWN) {
                    board->makeMove(keyEvent(event));
                    gGUI.renderScene(board->getBoard(), board->getScore());
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    this->PressButton();
                    gGUI.renderScene(board->getBoard(), board->getScore());
                }

            }
            uint64_t end = SDL_GetPerformanceCounter();

            float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

            SDL_Delay(floor(16.666f - elapsedMS));
        }
    }

    GameLogic::Direction Game::keyEvent(SDL_Event& E) {
        switch (E.key.keysym.sym) {
        case SDLK_UP:
            return GameLogic::Direction::up;
        case SDLK_DOWN:
            return GameLogic::Direction::down;
        case SDLK_LEFT:
            return GameLogic::Direction::left;
        case SDLK_RIGHT:
            return GameLogic::Direction::right;
        default:
            return GameLogic::Direction::none;
        }
    }

    void Game::PressButton() {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (undoBtn.withinBounds(x, y))
            board->undoMove();
        else if (redoBtn.withinBounds(x, y))
            board->redoMove();
        else if (restartBtn.withinBounds(x, y))
            this->restartGame();
    }
};