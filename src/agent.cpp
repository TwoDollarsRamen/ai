#include <cmath>

#include "agent.hpp"
#include "level.hpp"

agent::agent(SDL_Surface* atlas) :
	spr(atlas, {96, 96, 16, 16}),
	position(0, 0),
	current_target_idx(0),
	state(IDLE) {
}

void agent::tick(float ts, level& l) {
	switch (state) {
	case IDLE: {
		if (current_target_idx >= path.size() || path.empty()) {
			compute_path(l, l.get_random_node());
		}

		const auto& current = path[current_target_idx];

		const float distance_from_current = sqrt(pow(current.x - position.x, 2)
				+ pow(current.y - position.y, 2));

		if (distance_from_current > 1) {
			const auto direction = vec2(current.x - position.x, current.y - position.y).unit();

			position.x += direction.x * 40.0f * ts;
			position.y += direction.y * 40.0f * ts;
		} else {
			current_target_idx++;
		}
	}
	default: break;
	}
}

void agent::draw(const renderer& ren) {
	ren.draw(position, spr);
}

void agent::compute_path(level& level, const vec2& target) {
	path = level.find_path(
			(int)(position.x / grid_size),
			(int)(position.y / grid_size),
			(int)(target.x / grid_size),
			(int)(target.y / grid_size));
	current_target_idx = 0;

	for (auto& n : path) {
		n.x *= grid_size;
		n.y *= grid_size;
	}
}
