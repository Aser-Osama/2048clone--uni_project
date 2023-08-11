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
		mFont = TTF_OpenFont("CreativeMindProject\\assets\\LexendMega-Regular.ttf", 50);
		for (uint8_t i = 1; i < 15; i++) {
			std::string FilePath = "CreativeMindProject\\assets\\ (" + std::to_string(i) + ").bmp";
			textureMap[i] = this->loadTexture(FilePath.c_str());
		}
		std::string bgPath = "CreativeMindProject\\assets\\bg.bmp";
		this->bgImg = loadTexture(bgPath.c_str());

	}

	void gameGUI::renderScene(const std::vector<std::vector<GameLogic::Tile*>>& boardVec, 
		int score) {
		this->renderBG();
		this->renderText(score);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				GameLogic::Tile* t = (boardVec)[i][j];
				if (t) {
					int val = t->getVal();
					if (val != -1) {

						int x = (j)*TILE_W + 50 + (j * 4);
						int y = (i)*TILE_H + (i * 4);
						this->render(textureMap[val], x, y, TILE_H, TILE_W);
					}
				}
			}
		}

	}
	void gameGUI::renderBG() {
		this->render(bgImg, NULL, NULL, NULL, NULL);
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
		render(t, (826 + 100 - w/2), 250 + offsetdown, w, h);
		SDL_DestroyTexture(t);
	}

	gameGUI::~gameGUI() {
		for (auto& e : textureMap) {
			SDL_DestroyTexture(e.second);
		}
	}
};

