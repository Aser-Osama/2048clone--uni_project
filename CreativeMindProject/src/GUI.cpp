#include <SDL.h>
#include "GUI.h"
#include <iostream>
#include <string>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_W 180
#define TILE_H 180


#define GRID_SIZE 4
namespace GUI {
	//GUI definitions
	GUI::GUI(const char* pTitle, int pW, int pH): mWindow(NULL), mRenderer(NULL) {
		mWindow = SDL_CreateWindow(pTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pW, pH, SDL_WINDOW_SHOWN);
		if (!mWindow) {
			std::cout << "Window couldn't init: " << SDL_GetError() << std::endl;
		}
		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	}

	SDL_Texture* GUI::loadTexture(const char* pFilePath) {
		SDL_Surface* surface = SDL_LoadBMP(pFilePath);
		if (!surface) {
			std::cout << "Couldn't load image: " << SDL_GetError() << std::endl;
			return nullptr;
		}
		SDL_Texture* texture = NULL;
		texture = SDL_CreateTextureFromSurface(mRenderer, surface);
		SDL_FreeSurface(surface);
		if (!texture) {
			std::cout << "Couldn't create texture from surface: " << SDL_GetError() << std::endl;
			return nullptr;
		}
		return texture;
	}

	void GUI::clear() {
		SDL_RenderClear(mRenderer);
	}

	void GUI::render(SDL_Texture* pTexture, int x, int y) {
		SDL_Rect src;
		SDL_Rect dst;
		src.x = 0;
		src.y =0;
		dst.x = x;
		dst.y = y;
		src.w = dst.w = TILE_W;
		src.h = dst.h = TILE_H;
		SDL_RenderCopy(mRenderer, pTexture, &src, &dst);
	}

	void GUI::display() {
		SDL_RenderPresent(mRenderer);
	}

	GUI::~GUI(){
		SDL_DestroyWindow(mWindow);
	}


	//gameGUI definitions
	void gameGUI::loadTextures() {
		for (uint8_t i = 1; i < 15; i++) {
			std::string FilePath = "CreativeMindProject\\Imgs\\ (" + std::to_string(i) + ").bmp";
			textureMap[i] = this->loadTexture(FilePath.c_str());
		}

	}

	void gameGUI::renderTile(int val, int iI, int jI) {
		if (val != -1) {
			int x = (jI)*TILE_W + 50;
			int y = (iI)*TILE_H;
			this->render(textureMap[val], x, y);
		}
	}

	gameGUI::~gameGUI() {
		for (auto& e : textureMap) {
			SDL_DestroyTexture(e.second);
		}
	}
};

