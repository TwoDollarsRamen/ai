#pragma once

#include <stdint.h>

#define grid_size 16

struct vec2 {
	float x, y;

	vec2() : x(0.0f), y(0.0f) {}
	vec2(float x) : x(x), y(x) {}
	vec2(float x, float y) : x(x), y(y) {}
};
