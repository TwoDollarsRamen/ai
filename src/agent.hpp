#pragma once

#include "common.hpp"
#include "level.hpp"

struct agent {
	vec2 position;
	sprite spr;
	std::vector<vec2> path;
	int current_target_idx = 0;
	float lerp_t = 0.0;

	agent(SDL_Surface* atlas);

	void tick(float ts);
	void draw(const renderer& ren);
	void compute_path(level& level, const vec2& target);
};
