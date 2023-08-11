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
			void render(SDL_Texture* pTexture, int16_t x, int16_t y, int16_t sx, int16_t sy);
			GUI(const char* pTitle, int16_t pW, int16_t pH);
			SDL_Texture* loadTexture(const char* pFilePath);
	public:			
		virtual void clear();
		virtual void display();
		virtual ~GUI();
		};

	class gameGUI : public GUI {
	private:
		std::map<uint8_t, SDL_Texture*> textureMap;
		SDL_Texture* bgImg;
	public:
		gameGUI(const char* pTitle, int16_t pW, int16_t pH) : GUI(pTitle, pW, pH), bgImg(NULL) {}
		void loadTextures();
		void renderScene(const std::vector<std::vector<GameLogic::Tile*>>& boardVec, uint32_t);
		void renderBG();
		void renderText(uint32_t);
		~gameGUI();
	};
}
