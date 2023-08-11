#include "Game.h"
#include <SDL.h>
#include <Windows.h>
#include <iostream>
#include "GameLogic.h"
#include <vector>
#include <map>
namespace Game { 
    bool ButtonBounds::withinBounds(int x, int y)
    {
        return (x > this->startX && x < this->endX
            && y > this->startY && y < this->endY);
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
    }

    void Game::run() {
        bool gameRunning = true;
        SDL_Event event;
        while (gameRunning) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || board.isGameComplete()){
                    gameRunning = false;
                    break;
                }
                else if (event.type == SDL_KEYDOWN) {
                    board.makeMove(keyEvent(event));
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    this->PressButton();
                }

            }
            gGUI.clear();
            gGUI.renderScene(board.getBoard(), board.getScore());
            gGUI.display();
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
            board.undoMove();
        else if (redoBtn.withinBounds(x, y))
            board.redoMove();
    }
};