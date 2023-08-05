#pragma once
#include <SDL.h>
#include "GameLogic.h"
namespace GUI {
	class Game {
	private:
		GameLogic::Board* mBoard;
		SDL_Window* window;
		SDL_Surface* screenSurface;
	public:
		Game();
		void run();

	};
}
