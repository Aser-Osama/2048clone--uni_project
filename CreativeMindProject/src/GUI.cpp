#include <SDL.h>
#include <array>
#include <random>
#include "GUI.h"
#include <windows.h>
#include "GameLogic.h"
#include <iostream>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRID_SIZE 4
namespace GUI {
    Game::Game() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        }
        else {
            window = SDL_CreateWindow("2048 Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (window == NULL) {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            }
            else {
                screenSurface = SDL_GetWindowSurface(window);
                mBoard = new GameLogic::Board;
            }
        }
    }

    void Game::run() {
        bool quit = false;
        bool gameEnded = false;
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                    goto GameHasEnded;
                }
                //Checks if game is over
                else if (mBoard->isGameComplete()) {
                    gameEnded = true;
                    goto GameHasEnded;
                }

                // User presses a key
                else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        if(mBoard->makeMove('u'));
                            mBoard->addNewTile();
                        break;
                    case SDLK_DOWN:
                        if (mBoard->makeMove('d'));
                            mBoard->addNewTile();
                        break;
                    case SDLK_LEFT:
                        if (mBoard->makeMove('l'));
                            mBoard->addNewTile();
                        break;
                    case SDLK_RIGHT:
                        if (mBoard->makeMove('r'));
                            mBoard->addNewTile();
                        break;
                    }
                }
            }
            // Render the game state here
            mBoard->printv();
            Sleep(500);
            system("cls");
            

            // Update the window surface
            SDL_UpdateWindowSurface(window);
        }
    GameHasEnded:
        // Cleanup
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    };

