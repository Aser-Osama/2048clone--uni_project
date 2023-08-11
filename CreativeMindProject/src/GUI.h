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
			SDL_Surface* mScreenSurface;
			SDL_Renderer* mRenderer;
			TTF_Font* mFont;
			void render(SDL_Texture* pTexture, int x, int y, int sx, int sy);
			GUI(const char* pTitle, int pW, int pH);
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
		gameGUI(const char* pTitle, int pW, int pH) : GUI(pTitle, pW, pH), bgImg(NULL) {}
		void loadTextures();
		void renderScene(const std::vector<std::vector<GameLogic::Tile*>>& boardVec, int);
		void renderBG();
		void renderText(uint32_t);
		~gameGUI();
	};
}
