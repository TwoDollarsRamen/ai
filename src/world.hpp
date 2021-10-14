#pragma once

#include <string>

#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "agent.hpp"
#include "menu.hpp"

/* The objects are heap allocated instead of simply storing
 * objects on the stack so that the object lifetimes can
 * be controlled. */
struct world_state_play {
	level* l;
	player* p;
};

struct world_state_main_menu {
	menu* men;
	font* main_font;
};

/* Manages the player, level, and AI agents. */
class world {
public:
	enum class state {
		PLAY,
		MAIN_MENU
	} current_state;

private:
	std::string level_path;

	/* Used to avoid changing state while iterating the agents */
	bool iterating = false;
	bool want_change_state = false;
	state change_to;
public:
	bool want_quit = false;

	union {
		world_state_play play_state;
		world_state_main_menu main_menu_state;
	};

	world(const char* level_path);
	~world();

	void change_state(state s);

	void init_state(state s);
	void deinit_state();

	/* To be called in the `SDL_PollEvent' loop. */
	void update_events(const SDL_Event& e);

	void tick(float ts);
	void draw(const renderer& ren);
};
