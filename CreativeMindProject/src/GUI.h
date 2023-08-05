#pragma once
#include <SDL.h>
namespace GUI {
	class GUI {
		private:
			SDL_Window* mWindow;
			SDL_Surface* mScreenSurface;
			SDL_Renderer* mRenderer;
		public:
			GUI(const char* pTitle, int pW, int pH);
			SDL_Texture* loadTexture(const char* pFilePath);
			void clear();
			void render(SDL_Texture* pTexture);
			void display();
			~GUI();
		};
}
