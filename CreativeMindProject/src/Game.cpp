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
		gui = new GUI::GUI("2048 CLONE", 1280, 720);
		gui->loadTextures();
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
                        if (board.makeMove('u'))
                            board.addNewTile();

                        break;
                    case SDLK_DOWN:
                        if(board.makeMove('d'))
                            board.addNewTile();

                        break;
                    case SDLK_LEFT:
                        if(board.makeMove('l'))
                            board.addNewTile();

                        break;
                    case SDLK_RIGHT:
                        if(board.makeMove('r'))
                            board.addNewTile();

                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            gui->clear();
            std::vector<std::vector<GameLogic::Tile*>>* boardVec = board.getBoard();
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    GameLogic::Tile* t = (*boardVec)[i][j];
                    if (t != NULL) {
                        gui->renderTile(t->getVal(), i, j);
                    }
                    else {
                        continue;
                    }
                }
            }
            gui->display();
            
        }
    }
};