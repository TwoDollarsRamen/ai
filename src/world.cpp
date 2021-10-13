#include "world.hpp"

world::world(const char* level_path) {
	l.load(level_path);
}

void world::update_events(const SDL_Event& e) {
	p.update_events(e);
}

void world::tick(float ts) {
	p.tick(ts, l);

	for (auto& a : l.agents) {
		a.tick(ts, p, l);
	}
}

void world::draw(const renderer& ren) {
	l.draw(ren);

	p.draw(ren, l);

	for (auto& a : l.agents) {
		a.draw(ren);
	}
}
