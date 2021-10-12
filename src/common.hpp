#pragma once

#include <stdint.h>

#define grid_size 16

#include <SDL2/SDL.h>

struct vec2 {
	float x, y;

	vec2() : x(0.0f), y(0.0f) {}
	vec2(float x) : x(x), y(x) {}
	vec2(float x, float y) : x(x), y(y) {}

	float magnitude() const {
		return sqrt(x * x + y * y);
	}

	vec2 unit() const {
		float len = magnitude();
		return vec2(x / len, y / len);
	}
};

bool rect_overlap(const SDL_Rect& a, const SDL_Rect& b, vec2& normal);
