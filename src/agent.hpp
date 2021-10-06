#pragma once

#include "common.hpp"
#include "level.hpp"

struct agent {
	vec2 position;
	vec2 target;

	void tick(float ts);
	void recompute_path(const level& level);
};
