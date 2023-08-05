#include "Game.h"
#include <SDL.h>
#include <iostream>
#include "GameLogic.h"
#include "GUI.h"
#include <map>
namespace Game {
	GUI::GUI* gui;
	std::map<int, SDL_Texture*> textureMap;
	SDL_Texture* tt = NULL;
	void init() {
		if (SDL_Init(SDL_INIT_VIDEO) > 0) {
			std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
		}
		gui = new GUI::GUI("2048 CLONE", 1280, 720);
		tt = gui->loadTexture("CreativeMindProject\\Imgs\\ (3).bmp");
		if (tt == NULL) {
			std::cout << "Failed to load texture" << std::endl;
		}
		textureMap[2] = tt;  
		tt = NULL;
	}

	void run() {
		init();
		bool gameRunning = true;
		SDL_Event event;
		while (gameRunning) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					gameRunning = false; 
				}
			}

			gui->clear();
			gui->render(textureMap[2]);
			gui->display();

		}
	}
};