#pragma once

#include <vector>

#include "common.hpp"
#include "player.hpp"
#include "renderer.hpp"

/* Forward declaration to avoid a circular include, since
 * `level.hpp' includes this file. */
class level;

struct agent {
	enum {
		IDLE
	} state;

	vec2 position;
	sprite spr;
	std::vector<vec2> path;
	int current_target_idx = 0;

	agent(SDL_Surface* atlas);

	void tick(float ts, level& l);
	void draw(const renderer& ren);
	void compute_path(level& level, const vec2& target);
};
