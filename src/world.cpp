#include "world.hpp"

world::world(const char* level_path)
	: level_path(level_path) {
	init_state(state::MAIN_MENU);
}

world::~world() {
	deinit_state();
}

void world::update_events(const SDL_Event& e) {
	switch (current_state) {
	case state::PLAY:
		play_state.p->update_events(e);
		break;
	case state::MAIN_MENU:
		main_menu_state.men->handle_events(e);
		break;
	default: break;
	}
}

void world::tick(float ts) {
	iterating = true;

	switch (current_state) {
	case state::PLAY:
		play_state.p->tick(ts, *this, *play_state.l);

		for (auto& a : play_state.l->agents) {
			a.tick(ts, *play_state.p, *play_state.l);
		}
		break;
	default: break;
	}

	iterating = false;
	if (want_change_state) {
		change_state(change_to);
	}
}

void world::draw(const renderer& ren) {
	switch (current_state) {
	case state::PLAY:
		play_state.l->draw(ren);

		play_state.p->draw(ren, *play_state.l);

		for (auto& a : play_state.l->agents) {
			a.draw(ren);
		}
		break;
	case state::MAIN_MENU:
		main_menu_state.men->draw(ren);
		break;
	}
}

void world::change_state(world::state s) {
	if (iterating) {
		want_change_state = true;
		change_to = s;
	} else {
		deinit_state();
		init_state(s);
	}
}

void world::init_state(world::state s) {
	current_state = s;

	switch (s) {
	case state::PLAY:
		play_state.p = new player();
		play_state.l = new level();
		play_state.l->load(level_path.c_str());
		break;
	case state::MAIN_MENU:
		main_menu_state.main_font = new font("res/DejaVuSans.ttf", 25.0f);
		main_menu_state.men = new menu(*this, *main_menu_state.main_font,
			sprite {texture_manager::load("res/atlas.png"), { 135, 149, 13, 6}});

		main_menu_state.men->add_item("Play", [this](world& world){
			change_state(state::PLAY);
		});
		main_menu_state.men->add_item("Quit", [this](world& world){
			want_quit = true;
		});
		break;
	default: break;
	};
}

void world::deinit_state() {
	switch (current_state) {
	case state::PLAY:
		delete play_state.l;
		delete play_state.p;
		break;
	case state::MAIN_MENU:
		delete main_menu_state.men;
		delete main_menu_state.main_font;
	default: break;
	}
}
