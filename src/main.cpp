#include <SDL2/SDL.h>

#include "renderer.hpp"

i32 main(i32 argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600, SDL_WINDOW_SHOWN);

	renderer ren(window, 2);

	SDL_Surface* atlas = texture_manager::load("res/atlas.png");

	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		ren.clear();
		ren.draw(vec2(100, 100), atlas, { 0, 0, atlas->w, atlas->h});

		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
