#include "Game.h"
#include <SDL.h>
#include <Windows.h>
#include <iostream>
#include "GameLogic.h"
#include <vector>
#include <map>
namespace Game {

	void Game::init() {
		if (SDL_Init(SDL_INIT_VIDEO) > 0) {
			std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
		}
		gGUI = new GUI::gameGUI("2048 CLONE", 1280, 720);
		gGUI->loadTextures();
	}

    void Game::run() {
        init();
        bool gameRunning = true;
        SDL_Event event;
        while (gameRunning) {
            while (SDL_PollEvent(&event)) {
                if (board.isGameComplete()) {
                    gameRunning = false;
                    break;
                }
                switch (event.type) {
                case SDL_QUIT:
                    gameRunning = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        board.makeMove('u');
                        break;
                    case SDLK_DOWN:
                        board.makeMove('d');
                        break;
                    case SDLK_LEFT:
                        board.makeMove('l');
                        break;
                    case SDLK_RIGHT:
                        board.makeMove('r');
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
            gGUI->clear();
            std::vector<std::vector<GameLogic::Tile*>>* boardVec = board.getBoard();
            gGUI->renderBG();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    GameLogic::Tile* t = (*boardVec)[i][j];
                    if (t) {
                        gGUI->renderTile(t->getVal(), i, j);
                    }
                }
            }
            gGUI->display();
        }
    }
};