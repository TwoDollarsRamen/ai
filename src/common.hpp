#pragma once

#include <stdint.h>

struct vec2 {
	float x, y;

	vec2(float x) : x(x), y(x) {}
	vec2(float x, float y) : x(x), y(y) {}
};
