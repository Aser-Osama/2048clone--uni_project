#include <SDL.h>
#include <array>
#include <random>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRID_SIZE 4
 
//test

class Board {
public:
    std::array<std::array<int, GRID_SIZE>, GRID_SIZE> grid;

    Board() {
        for(int i = 0; i < GRID_SIZE; i++) {
            for(int j = 0; j < GRID_SIZE; j++) {
                grid[i][j] = 0;
            }
        }

        // Add an initial tile
        addRandomTile();
    }

    void addRandomTile() {
        std::vector<std::pair<int, int>> emptyCells;

        // Find all empty cells
        for(int i = 0; i < GRID_SIZE; i++) {
            for(int j = 0; j < GRID_SIZE; j++) {
                if(grid[i][j] == 0) {
                    emptyCells.push_back(std::make_pair(i, j));
                }
            }
        }

        // Choose a random cell
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, emptyCells.size() - 1);

        int randomCellIndex = distrib(gen);
        std::pair<int, int> randomCell = emptyCells[randomCellIndex];

        // Set the cell to 2 or 4
        grid[randomCell.first][randomCell.second] = (gen() % 2 + 1) * 2;
    }

    // Other board methods will go here
};

class Game {
public:
    SDL_Window* window;
    SDL_Surface* screenSurface;
    Board* board;

    Game() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        } else {
            window = SDL_CreateWindow("2048 Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
            if (window == NULL) {
                printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            } else {
                screenSurface = SDL_GetWindowSurface(window);
                board = new Board();
            }
        }
    }

    void run() {
        bool quit = false;
        SDL_Event e;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                // User requests quit
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                // User presses a key
                else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            // Move tiles up
                            break;
                        case SDLK_DOWN:
                            // Move tiles down
                            break;
                        case SDLK_LEFT:
                            // Move tiles left
                            break;
                        case SDLK_RIGHT:
                            // Move tiles right
                            break;
                    }
                }
            }

            // Render the game state here

            // Update the window surface
            SDL_UpdateWindowSurface(window);
        }

        // Cleanup
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

int main(int argc, char* args[]) {
    Game game;
    game.run();
    return 0;
}