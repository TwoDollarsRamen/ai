#include "world.hpp"

world::world(const char* level_path) {
	l.load(level_path);
}

void world::update_events(const SDL_Event& e) {
	p.update_events(e);
}

void world::tick(float ts) {
	p.tick(ts);
	p.resolve_collisions(l.collisions);

	for (auto& a : agents) {
		a.tick(ts);
	}
}

void world::draw(const renderer& ren) {
	l.draw(ren);

	p.draw(ren);

	for (auto& a : agents) {
		a.draw(ren);
	}
}
