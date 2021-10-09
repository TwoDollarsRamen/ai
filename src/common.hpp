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

bool rect_overlap(const SDL_Rect& a, const SDL_Rect& b, vec2& normal);
