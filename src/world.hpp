#pragma once

#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "agent.hpp"

/* Manages the player, level, and AI agents. */
struct world {
	level l;
	player p;

	std::vector<agent> agents;

	world(const char* level_path);

	/* To be called in the `SDL_PollEvent' loop. */
	void update_events(const SDL_Event& e);

	void tick(float ts);
	void draw(const renderer& ren);
};
