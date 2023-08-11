#include "Game.h"
#include <SDL.h>
#include <Windows.h>
#include <iostream>
#include "GameLogic.h"
#include <vector>
#include <map>
namespace Game { 
    Game::Game() : gGUI("2048 CLONE", 1280, 720)
    {
        if (SDL_Init(SDL_INIT_VIDEO) > 0) {
            std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
            exit(1);
        }
        gGUI.loadTextures();
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
                    //m_oldboard = board.getboard
                    board.makeMove(keyEvent(event));
                }
                if (event.key.keysym.sym == SDLK_TAB) {
                    this->board.undoMove();
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    std::cout << "x = " << x << "y = " << y << "\n"; //will start mapping the area of the buttons later
                }

            }
            gGUI.clear();
            gGUI.renderScene(board.getBoard());
            gGUI.display();
        }
    }
};