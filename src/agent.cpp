#include <cmath>

#include "agent.hpp"
#include "level.hpp"

agent::agent(SDL_Surface* atlas) :
	spr(atlas, {96, 96, 16, 16}),
	position(0, 0),
	current_target_idx(0),
	current_state(PURSUE),
	
	speed(40.0f),
	range(50.0f),

	collider(0, 0, 12, 15) {

}

void agent::tick(float ts, const player& player, level& l) {
	switch (current_state) {
	case IDLE: {
		if (current_target_idx >= path.size() || path.empty()) {
			compute_path(l, l.get_random_node());
		}

		const auto& current = path[current_target_idx];

		const float distance_from_current = sqrtf(powf(current.x - position.x, 2.0f)
				+ powf(current.y - position.y, 2.0f));

		const auto direction = vec2(current.x - position.x, current.y - position.y).unit();

		if (distance_from_current > 1) {
			position.x += direction.x * speed * ts;
			position.y += direction.y * speed * ts;
		} else {
			current_target_idx++;
		}

		if (cone_of_vision(player, direction)) {
			switch_state(PURSUE);
		}

		break;
	}
	case PURSUE: {
		const auto direction = vec2(player.position.x - position.x, player.position.y - position.y).unit();

		position.x += direction.x * speed * ts;
		position.y += direction.y * speed * ts;

		l.resolve_collisions_with_body(collider, position);

		if (!cone_of_vision(player, direction)) {
			compute_path(l, player.position);
			switch_state(IDLE);
		}

		break;
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

void agent::switch_state(const agent::state& newstate) {
	if (current_state == IDLE) {
		path.clear();
	}

	current_state = newstate;
}

bool agent::cone_of_vision(const player& player, const vec2& direction) const {
	const float distance_from_player = sqrtf(powf(player.position.x - position.x, 2)
			+ powf(player.position.y - position.y, 2));

	if (distance_from_player < range) {	
		const auto dir_to_player = vec2(player.position.x - position.x, player.position.y - position.y).unit();

		float angle = vec2::angle(dir_to_player, direction);
		if (angle < 5.0) {
			return true;
		}

		return false;
	}

	return false;
}
