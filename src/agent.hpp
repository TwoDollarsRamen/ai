#pragma once

#include <vector>

#include "common.hpp"
#include "player.hpp"
#include "renderer.hpp"

/* Forward declaration to avoid a circular include, since
 * `level.hpp' includes this file. */
class level;

struct agent {
	/* In the `IDLE' state, agents will pathfind to random
	 * locations on the level. When the player enters their
	 * field of vision, they will enter the `PURSUE' state.
	 *
	 * In the `PURSUE' state, agents walk directly towards
	 * the player. If the player exits their field of vision,
	 * they will go back to the `IDLE' state. */
	enum state {
		IDLE,
		PURSUE
	} current_state;

	vec2 position;
	sprite spr;
	std::vector<vec2> path;
	int current_target_idx = 0;

	SDL_Rect collider;

	float speed;
	float range; /* The range of the cone of vision. */

	agent(SDL_Surface* atlas);

	void tick(float ts, const player& player, level& l);
	void draw(const renderer& ren);
	void compute_path(level& level, const vec2& target);

	void switch_state(const state& newstate);

	/* Check if the player is inside the cone of vision. */
	bool cone_of_vision(const player& player, const vec2& direction) const;
};
