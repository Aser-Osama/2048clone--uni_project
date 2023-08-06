#pragma once
#include <SDL.h>
#include "GUI.h"
#include "GameLogic.h"
namespace Game
{
	class Game {
	public:
		void init();
		void run();
	private:
		GUI::gameGUI* gGUI;
		GameLogic::Board board;
	};


};

