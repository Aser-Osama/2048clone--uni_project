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

	void gameGUI::renderMovement(const std::vector<std::vector<GameLogic::Tile*>>& boardVec, uint32_t score)
	{
		int index = 0;
		int movedindex = 0;
		GameLogic::Tile* Tiles[16]{};
		GameLogic::Tile* TilesMoved[16]{};

		for (int8_t i = 0; i < 4; i++) {
			for (int8_t j = 0; j < 4; j++) {
				GameLogic::Tile* t = (boardVec)[i][j];
				if (t) {
					int8_t val = t->getVal();
					if (val != -1) {
						if (t->hasMoved())
							TilesMoved[movedindex++] = t;
						else
							Tiles[index++] = t;
					}
				}
			}
		}

		for (size_t ii = 0; ii < 20; ii++)
		{
			uint64_t start = SDL_GetPerformanceCounter();
			this->clear();
			this->renderBG();
			this->renderScore(score);
			
			for (GameLogic::Tile* t : TilesMoved)
			{
				for (GameLogic::Tile* t : TilesMoved)
				{
					if (!t)
						break;

					int i, j, val;
					int o_i, o_j, o_val;
					int px, pval;
					SDL_Rect dst, olddst, src;
					t->getValues(&i, &j, &val);
					t->getOldValues(&o_i, &o_j, &o_val);
					
					if (o_j > j)
						px = ((o_j)*TILE_W + 50 + (o_j * 4)) - (((o_j * TILE_W + 50 + (o_j * 4)) - (j * TILE_W + 50 + (j * 4))) * ii / 19);
					else
						px = ((o_j)*TILE_W + 50 + (o_j * 4)) + (((j * TILE_W + 50 + (j * 4)) - (o_j * TILE_W + 50 + (o_j * 4))) * ii/19);

					if (t->hasMerged())
						pval = o_val;
					else
						pval = val;

					if (ii == 19){
						pval = val;
						px = (j)*TILE_W + 50 + (j * 4);
					}

					dst = {
						dst.x = px,
						dst.y = (i)*TILE_H + (i * 4),
						dst.w = TILE_W,
						dst.h = TILE_H };

					src = {
						src.x = (pval - 1) * 177,
						src.y = 0,
						src.w = TILE_W,
						src.h = TILE_H };
					this->render(tileTexture, &src, &dst);
				}
			}

			for (GameLogic::Tile* t : Tiles)
			{ //this will remain unchanged.
				if (!t)
					continue;

				int i, j, val;
				SDL_Rect dst, olddst, src;
				//if (t->madeFirstMove())
				//	t->getOldValues(&i, &j, &val);
				//else
					t->getValues(&i, &j, &val);

				dst = {
					dst.x = (j)*TILE_W + 50 + (j * 4),
					dst.y = (i)*TILE_H + (i * 4),
					dst.w = TILE_W,
					dst.h = TILE_H };

				src = {
					src.x = (val - 1) * 177,
					src.y = 0,
					src.w = TILE_W,
					src.h = TILE_H };
				this->render(tileTexture, &src, &dst);
			}
			uint64_t end = SDL_GetPerformanceCounter();
			float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
			std::cout << "Current FPS: " << std::to_string(1.0f / elapsedMS) << std::endl;
			// Cap to 60 FPS
			this->display();
			if (floor(16.666f - elapsedMS) > 0)
				SDL_Delay(floor(16.666f - elapsedMS));
		}
	
		/*for (int8_t i = 0; i < 4; i++) {
			for (int8_t j = 0; j < 4; j++) {
				GameLogic::Tile* t = (boardVec)[i][j];
				if (t) {
					int8_t val = t->getVal();
					if (val != -1) {
						int i, j, val;
						SDL_Rect dst, olddst, src;
						t->getValues(&i, &j, &val);

						dst = {
							dst.x = (j)*TILE_W + 50 + (j * 4),
							dst.y = (i)*TILE_H + (i * 4),
							dst.w = TILE_W,
							dst.h = TILE_H };

						src = {
							src.x = (val - 1) * 177,
							src.y = 0,
							src.w = TILE_W,
							src.h = TILE_H };

						this->render(tileTexture, &src, &dst);
					}
				}
			}
		}*/
	}

	void gameGUI::renderScene(const std::vector<std::vector<GameLogic::Tile*>>& boardVec,
		uint32_t score) {
		this->clear();
		this->renderBG();
		this->renderScore(score);
		this->renderMovement(boardVec, score);
		this->display();
	}

	void gameGUI::renderBG() {
		this->render(bgImg, NULL, NULL);
	}

	void gameGUI::renderScore(uint32_t score)
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

