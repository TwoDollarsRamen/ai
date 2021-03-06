#include "player.hpp"
#include "level.hpp"
#include "world.hpp"

player::player() :
	spr(texture_manager::load("res/atlas.png"), SDL_Rect { 3, 65, 10, 15 }),
	position(32.0f, 64.0f),
	collider(0, 0, 10, 15),
	speed(50.0f),

	up_pressed(false),
	down_pressed(false),
	left_pressed(false),
	right_pressed(false) {}

void player::update_events(const SDL_Event& e) {
	switch (e.type) {
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym) {
		case SDLK_UP:    up_pressed    = true; break;
		case SDLK_DOWN:  down_pressed  = true; break;
		case SDLK_LEFT:	 left_pressed  = true; break;
		case SDLK_RIGHT: right_pressed = true; break;
		}
		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym) {
		case SDLK_UP:    up_pressed    = false; break;
		case SDLK_DOWN:  down_pressed  = false; break;
		case SDLK_LEFT:	 left_pressed  = false; break;
		case SDLK_RIGHT: right_pressed = false; break;
		}
		break;
	default: break;
	}
}

void player::tick(float ts, world& w, level& l) {
	if (left_pressed) {
		position.x -= speed * ts;
	}
	if (right_pressed) {
		position.x += speed * ts;
	}
	if (up_pressed) {
		position.y -= speed * ts;
	}
	if (down_pressed) {
		position.y += speed * ts;
	}

	l.resolve_collisions_with_body(collider, position);

	SDL_Rect player_rect = {
		(int)position.x + collider.x,
		(int)position.y + collider.y,
		collider.w, collider.h
	};

	/* Check for colllisions with the key */
	for (const auto& k : l.keys) {
		if (rect_overlap(player_rect, k)) {
			has_key = true;

			/* Clear the keys vector to create the illusion of
			 * it being destroyed.
			 *
			 * Only one key is expected, so this is fine.*/
			l.keys.clear();
		}
	}

	/* Check for collisions with the door. */
	if (has_key) {
		for (const auto& d : l.doors) {
			if (rect_overlap(player_rect, d)) {
				w.win = true;
				w.change_state(world::state::GAME_OVER);
			}
		}
	}

	/* Check for collisions with agents */
	for (const auto& a : l.agents) {
		SDL_Rect agent_rect = {
			a.collider.x + (int)a.position.x,
			a.collider.y + (int)a.position.y,
			(int)a.collider.w, (int)a.collider.h};

		if (rect_overlap(player_rect, agent_rect)) {
			w.win = false;
			w.change_state(world::state::GAME_OVER);
		}
	}
}

void player::draw(const renderer& ren, const level& l) {
	SDL_Surface* backbuffer = SDL_GetWindowSurface(ren.window);

	/* Position the camera so that the player is in the middle of the screen. */
	ren.camera_position.x = position.x - (backbuffer->w / (2 * ren.pixel_size));
	ren.camera_position.y = position.y - (backbuffer->h / (2 * ren.pixel_size));

	ren.draw(position, spr);

	if (has_key) {
		ren.draw(vec2(0, 0), l.key_sprite, true);
	}
}

