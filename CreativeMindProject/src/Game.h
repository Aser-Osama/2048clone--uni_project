#pragma once
#include <SDL.h>
#include "GUI.h"
#include "GameLogic.h"
namespace Game
{
	class Game {
	private:
		GUI::gameGUI* gGUI;
		void init();
		GameLogic::Board board;
		GameLogic::Direction keyEvent(SDL_Event& E);
	public:
		void run();
	};


};

