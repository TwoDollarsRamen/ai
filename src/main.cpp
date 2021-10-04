#include <SDL2/SDL.h>

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* window = SDL_CreateWindow("game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600, SDL_WINDOW_SHOWN);

	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}


		SDL_Surface* backbuffer = SDL_GetWindowSurface(window);

		/* Clear the screen by filling the backbuffer with a black rectangle. */
		SDL_Rect screen_rect = { 0 };
		SDL_GetWindowSize(window, &screen_rect.w, &screen_rect.h);
		SDL_FillRect(backbuffer, &screen_rect, 0x0);

		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
