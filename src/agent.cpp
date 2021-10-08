#include <cmath>

#include "agent.hpp"

agent::agent(SDL_Surface* atlas) : spr(atlas, {96, 96, 16, 16}) {}

void agent::tick(float ts) {
	lerp_t += ts;

	const auto& current = path[current_target_idx];

	position.x = std::lerp(position.x, current.x, (float)lerp_t);
	position.y = std::lerp(position.y, current.y, (float)lerp_t);

	if (lerp_t >= 1.0) {
		lerp_t = 0.0;
		if (current_target_idx < path.size() - 1) {
			current_target_idx++;
		}
	}
}

void agent::draw(const renderer& ren) {
	ren.draw(position, spr);
}

void agent::compute_path(level& level, const vec2& target) {
	path = level.find_path((int)(position.x / grid_size), (int)(position.y / grid_size),
			target.x, target.y);
	if (current_target_idx >= path.size()) {
		current_target_idx = 0;
	}

	for (auto& n : path) {
		n.x *= grid_size;
		n.y *= grid_size;
	}
}
