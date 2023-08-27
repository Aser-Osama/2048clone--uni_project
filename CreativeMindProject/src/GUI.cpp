#include <SDL.h>
#include "GUI.h"
#include <iostream>
#include <string>
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_W 177
#define TILE_H 177

namespace GUI {
	//GUI definitions
	GUI::GUI(const char* pTitle, int16_t pW, int16_t pH) : mWindow(NULL), mRenderer(NULL), mFont(NULL) {
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

	void GUI::render(SDL_Texture* pTexture, SDL_Rect* src, SDL_Rect* dst) {
		SDL_RenderCopy(mRenderer, pTexture, src, dst);
	}

	void GUI::display() {
		SDL_RenderPresent(mRenderer);
	}

	GUI::~GUI() {
		SDL_DestroyWindow(mWindow);
	}


	//gameGUI definitions
	void gameGUI::loadTextures() {

		mFont = TTF_OpenFont("assets\\LexendMega-Regular.ttf", 50);
		tileTexture = this->loadTexture("assets\\tiles.bmp");
		this->bgImg = loadTexture("assets\\bg.bmp");
	}

	void gameGUI::renderScene(const std::vector<std::vector<GameLogic::Tile*>>& boardVec,
		uint32_t score) {
		this->renderBG();
		this->renderText(score);
		for (int8_t i = 0; i < 4; i++) {
			for (int8_t j = 0; j < 4; j++) {
				GameLogic::Tile* t = (boardVec)[i][j];
				if (t) {
					int8_t val = t->getVal();
					if (val != -1) {
						SDL_Rect dst{
							dst.x = (j)*TILE_W + 50 + (j * 4),
							dst.y = (i)*TILE_H + (i * 4),
							dst.w = TILE_W,
							dst.h = TILE_H };
						SDL_Rect src{
							src.x = (val - 1) * 177,
							src.y = 0,
							src.w = TILE_W,
							src.h = TILE_H };
						this->render(tileTexture, &src, &dst);
					}
				}
			}
		}

	}
	void gameGUI::renderBG() {
		this->render(bgImg, NULL, NULL);
	}

	void gameGUI::renderText(uint32_t score)
	{
		std::string str;
		int w, h;
		int offsetdown = 0;
		if (score >= 100000) {
			offsetdown = 15;
			TTF_SetFontSize(mFont, 35);
		}
		str = std::to_string(score);
		SDL_Color c{ 0,0,0,255 };
		SDL_Surface* s = TTF_RenderText_Blended(mFont, str.c_str(), c);
		SDL_Texture* t = SDL_CreateTextureFromSurface(mRenderer, s);
		SDL_FreeSurface(s);
		SDL_QueryTexture(t, NULL, NULL, &w, &h);
		SDL_Rect dst{
			dst.x = (826 + 100 - w / 2),
			dst.y = 250 + offsetdown,
			dst.w = w,
			dst.h = h,
		};
		render(t, NULL, &dst);
		SDL_DestroyTexture(t);
	}

	gameGUI::~gameGUI() {
		SDL_DestroyTexture(tileTexture);
	}
};

