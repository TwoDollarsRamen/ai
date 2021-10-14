#pragma once

#include <vector>

#include "renderer.hpp"

class level;
class world;

/* Manages the player and its associated state. */
class player {
private:
	sprite spr;

	float speed;

	/* Movement key states. */
	bool up_pressed,
	     down_pressed,
	     left_pressed,
	     right_pressed;

	bool has_key = false;
public:
	SDL_Rect collider;
	vec2 position;

	player();

	void update_events(const SDL_Event& e);
	void tick(float ts, world& w, level& l);
	void draw(const renderer& ren, const level& l);
};
