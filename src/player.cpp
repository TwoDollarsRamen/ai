#include "player.hpp"

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

void player::tick(float ts) {
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
}

void player::draw(const renderer& ren) {
	ren.draw(position, spr);
}

void player::resolve_collisions(const std::vector<SDL_Rect>& rects) {
	SDL_Rect player_rect = {
		(int)position.x + collider.x,
		(int)position.y + collider.y,
		collider.w, collider.h
	};

	for (const auto& rect : rects) {
		vec2 normal;
		if (rect_overlap(player_rect, rect, normal)) {
			if (normal.x == 1) {
				position.x = rect.x - player_rect.w;
			} else if (normal.x == -1) {
				position.x = rect.x + rect.w;
			} else if (normal.y == 1) {
				position.y = rect.y - player_rect.h;
			} else if (normal.y == -1) {
				position.y = rect.y + rect.h;
			} 
		}
	}
}
