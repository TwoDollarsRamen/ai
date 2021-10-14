#pragma once

#include <string>

#include "level.hpp"
#include "player.hpp"
#include "renderer.hpp"
#include "agent.hpp"
#include "menu.hpp"

/* Objects in the state structs are heap allocated instead
 * of simply storing objects on the stack so that the object
 * lifetimes can be controlled.
 *
 * This struct is for the main `play' state of the game. */
struct world_state_play {
	level* l;
	player* p;
};

/* The begin menu state. */
struct world_state_main_menu {
	menu* men;
	font* main_font;
};

/* The ending menu state. */
struct world_state_game_over {
	menu* men;
	font* men_font;
	font* title_font;
};

/* Manages the player, level, and AI agents. */
class world {
public:
	enum class state {
		PLAY,
		MAIN_MENU,
		GAME_OVER
	} current_state;

private:
	std::string level_path;

	/* Used to avoid changing state while iterating the agents,
	 * which can cause undefined behaviour. */
	bool iterating = false;
	bool want_change_state = false;
	state change_to;
public:
	bool want_quit = false;

	bool win; /* Whether to show `you win' or `you lose' text on the game over screen. */

	union {
		world_state_play play_state;
		world_state_main_menu main_menu_state;
		world_state_game_over game_over_state;
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
