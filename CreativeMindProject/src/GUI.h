#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include "GameLogic.h"
#include <vector>
namespace GUI {
	class GUI {
	protected:
		SDL_Window* mWindow;
		SDL_Renderer* mRenderer;
		TTF_Font* mFont;
		void render(SDL_Texture* pTexture, SDL_Rect*, SDL_Rect*);
		GUI(const char* pTitle, int16_t pW, int16_t pH);
		SDL_Texture* loadTexture(const char* pFilePath);
	public:			
		virtual void clear();
		virtual void display();
		virtual ~GUI();
		};

	class gameGUI : public GUI {
	private:
		SDL_Texture* tileTexture;
		SDL_Texture* bgImg;
	public:
		gameGUI(const char* pTitle, int16_t pW, int16_t pH) : GUI(pTitle, pW, pH), bgImg(NULL) {}
		void loadTextures();
		void renderScene(const std::vector<std::vector<GameLogic::Tile*>>& boardVec, uint32_t);
		void renderMovement(const std::vector<std::vector<GameLogic::Tile*>>& boardVec, uint32_t score);
		void renderBG();
		void renderScore(uint32_t);
		~gameGUI();
	};
}
