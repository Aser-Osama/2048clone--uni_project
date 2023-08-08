#pragma once
#include <SDL.h>
#include "GUI.h"
#include "GameLogic.h"
namespace Game
{
	class Game {
	private:
		GUI::gameGUI* gGUI;
		GameLogic::Board board;
		void init();
	public:
		void run();
	};


};

