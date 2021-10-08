#pragma once

#include <stdint.h>

#define grid_size 16

#include <SDL2/SDL.h>

struct vec2 {
	float x, y;

	vec2() : x(0.0f), y(0.0f) {}
	vec2(float x) : x(x), y(x) {}
	vec2(float x, float y) : x(x), y(y) {}
};

static inline bool rect_overlap(SDL_Rect a, SDL_Rect b) {
	return (
		a.x + a.w > b.x &&
		a.y + a.h > b.y &&
		a.x < b.x + b.w &&
		a.y < b.y + b.h);
}
