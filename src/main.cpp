#include <SDL2/SDL.h>

#include "level.hpp"
#include "renderer.hpp"
#include "agent.hpp"
#include "world.hpp"

int main(int argc, char** argv) {
	srand(time(0));

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("game",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			800, 600, SDL_WINDOW_SHOWN);

	renderer ren(window, 2);

	uint64_t now = 0, last = 0;
	double ts = 0.0;

	world w("res/level.dat");

	bool running = true;
	while (running) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}

			w.update_events(e);
		}

		now = SDL_GetPerformanceCounter();
		ts = (float)(now - last) / (double)SDL_GetPerformanceFrequency();
		last = now;

		if (ts > 1.0f) {
			ts = 1.0f;
		}

		w.tick(ts);

		ren.clear();

		w.draw(ren);

		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
