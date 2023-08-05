#pragma once
#include <SDL.h>
#include <map>
namespace GUI {
	class GUI {
		private:
			SDL_Window* mWindow;
			SDL_Surface* mScreenSurface;
			SDL_Renderer* mRenderer;
			std::map<uint8_t, SDL_Texture*> textureMap;
			void render(SDL_Texture* pTexture, int x, int y);
		public:
			GUI(const char* pTitle, int pW, int pH);
			SDL_Texture* loadTexture(const char* pFilePath);
			void clear();
			void display();
			void loadTextures();
			void renderTile(int val, int x, int y);
			~GUI();
		};
}
