#include <SDL2/SDL.h>

#include "level.hpp"
#include "renderer.hpp"

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600, SDL_WINDOW_SHOWN);

	renderer ren(window, 2);

	SDL_Surface* atlas = texture_manager::load("res/atlas.png");

	level l;
	l.load("res/level.dat");

	auto path = l.find_path(3, 3, 5, 5);

	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}

		ren.clear();

		l.draw(ren);

		for (const auto& node : path) {
			ren.draw(vec2(node.x * 16, node.y * 16), atlas, { 64, 144, 16, 16 });
		}

		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
