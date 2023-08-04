
/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	
	
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("2048 Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Creating and loading an image
			SDL_Surface* imgSurface = NULL;
			//for vscode:
			imgSurface = SDL_LoadBMP("CreativeMindProject\\Imgs\\Tile_1.bmp");
			//for vs:
			//imgSurface = SDL_LoadBMP("Imgs\\Tile_1.bmp");
			
			

			if (imgSurface == NULL) {
				printf("Image could not be Loaded! SDL_Error: %s\n", SDL_GetError());
				return 1;
			}

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Hack to get window to stay up
			SDL_Event e;
			bool quit = false;
			SDL_Rect r{20,20,50,50};
			while (quit == false)
			{
				while (SDL_PollEvent(&e))
				{
					if (e.type == SDL_QUIT){

						quit = true; 
					}

					else if (e.type == SDL_KEYDOWN)
					{
						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							std::cout << "Up was pressed";
							r.y -=50; 
							break;
						case SDLK_DOWN:
							std::cout << "Down was pressed";
							r.y +=50; 

							break;
						case SDLK_RIGHT:
						
							std::cout << "Right was pressed";
							r.x +=50; 

							break;
						case SDLK_LEFT:
							std::cout << "Left was pressed";
							r.x -=50; 

							break;							
						default:
							break;
						}
					}
				}
				SDL_BlitSurface(imgSurface, &r, screenSurface, NULL);
				SDL_UpdateWindowSurface(window);
				SDL_UpdateWindowSurfaceRects(window, &r, 1);
			}
			SDL_FreeSurface(imgSurface);
			SDL_FreeSurface(screenSurface);

			imgSurface = NULL;
			screenSurface = NULL;
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
