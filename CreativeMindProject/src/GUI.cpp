#include <SDL.h>
#include "GUI.h"
#include <iostream>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_W (SCREEN_HEIGHT < SCREEN_WIDTH) ? ((SCREEN_HEIGHT - 4) / 4) : ((SCREEN_WIDTH - 4) / 4)
#define TILE_H (SCREEN_HEIGHT < SCREEN_WIDTH) ? ((SCREEN_HEIGHT - 4) / 4) : ((SCREEN_WIDTH - 4) / 4)


#define GRID_SIZE 4
namespace GUI {
	GUI::GUI(const char* pTitle, int pW, int pH): mWindow(NULL), mRenderer(NULL)
	{
		mWindow = SDL_CreateWindow(pTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, pW, pH, SDL_WINDOW_SHOWN);
		
		
		if (!mWindow)
		{
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
	void GUI::render(SDL_Texture* pTexture) {
		SDL_Rect src;
		src.x = 0;
		src.y =0;
		src.w = TILE_W;
		src.h = TILE_H;
		SDL_Rect dst;
		dst.x = 50;
		dst.y = 0;
		dst.w = TILE_W;
		dst.h = TILE_H;


		SDL_RenderCopy(mRenderer, pTexture, &src, &dst);
	}
	void GUI::display() {
		SDL_RenderPresent(mRenderer);
	}
	GUI::~GUI(){
		SDL_DestroyWindow(mWindow);
	}
};

