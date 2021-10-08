#pragma once

#include <vector>

#include "renderer.hpp"

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
public:
	vec2 position;
	vec2 velocity;

	player();

	void update_events(const SDL_Event& e);
	void tick(float ts);
	void draw(const renderer& ren);
	void resolve_collisions(const std::vector<SDL_Rect>& rects);
};
