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
		GUI::GUI* gui;
		GameLogic::Board board;
	};


};

