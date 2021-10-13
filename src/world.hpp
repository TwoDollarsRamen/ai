#pragma once

#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "agent.hpp"

/* A blackboard event */
struct event {
	union {
		vec2 player_spotted_event;
	} as;
};

/* Manages the player, level, and AI agents. */
struct world {
	level l;
	player p;

	std::vector<event> blackboard;

	world(const char* level_path);

	/* To be called in the `SDL_PollEvent' loop. */
	void update_events(const SDL_Event& e);

	void tick(float ts);
	void draw(const renderer& ren);
};
