#include <SDL.h>
#include "GUI.h"
#include <iostream>
#include <string>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_W 177
#define TILE_H 177


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

	void GUI::render(SDL_Texture* pTexture, int x, int y, int sx, int sy) {
		SDL_Rect src;
		SDL_Rect dst;
		src.x = 0;
		src.y =0;
		if (sx != NULL || sy != NULL) {
			dst.x = x;
			dst.y = y;
			src.w = dst.w = sx;
			src.h = dst.h = sy;
			SDL_RenderCopy(mRenderer, pTexture, &src, &dst);
		}
		else {
			SDL_RenderCopy(mRenderer, pTexture, NULL, NULL);
		}
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
		std::string bgPath = "CreativeMindProject\\Imgs\\bg.bmp";
		this->bgImg = loadTexture(bgPath.c_str());

	}

	void gameGUI::renderTile(int val, int iI, int jI) {
		if (val != -1) {
			int x = (jI)*TILE_W + 50 + (jI *4);
			int y = (iI)*TILE_H + (iI * 4);
			this->render(textureMap[val], x, y, TILE_H, TILE_W);
		}
	}
	void gameGUI::renderBG() {
		this->render(bgImg, NULL, NULL, NULL, NULL);
	}

	gameGUI::~gameGUI() {
		for (auto& e : textureMap) {
			SDL_DestroyTexture(e.second);
		}
	}
};

