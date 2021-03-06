#pragma once

#include <stdint.h>

#define grid_size 16

#include <SDL2/SDL.h>

/* A basic 2D vector class. */
struct vec2 {
	float x, y;

	vec2() : x(0.0f), y(0.0f) {}
	vec2(float x) : x(x), y(x) {}
	vec2(float x, float y) : x(x), y(y) {}

	/* Returns the magnitude of the vector. */
	float magnitude() const {
		return sqrtf(x * x + y * y);
	}

	/* Returns a new vector that is a normalised
	 * version of this vector, or a `unit' vector. */
	vec2 unit() const {
		float len = magnitude();
		return vec2(x / len, y / len);
	}

	float dot(const vec2& other) const {
		return x * other.x + y * other.y;
	}

	static float angle(const vec2& a, const vec2& b) {
		return 1.0f - (cosf(a.dot(b) / (a.magnitude() * b.magnitude())));
	}
};

/* Simple AABB collision detection.
 *
 * If the rectangles are overlapping, the normal
 * of the collision is placed in `normal', useful
 * for collision resolution. There is also a version
 * that simply checks for an overlap, without outputting
 * the normal; This is useful when the collision is not
 * going to be resolved. */
bool rect_overlap(const SDL_Rect& a, const SDL_Rect& b, vec2& normal);
bool rect_overlap(const SDL_Rect& a, const SDL_Rect& b);
