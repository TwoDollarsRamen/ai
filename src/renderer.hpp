#pragma once

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "common.hpp"

struct sprite {
	SDL_Surface* surface;
	SDL_Rect rect;
	bool flip = false;

	sprite(SDL_Surface* surface, SDL_Rect rect) : surface(surface), rect(rect) {}
};

class renderer {
private:
	mutable SDL_Window* window;

	const int pixel_size;
public:
	renderer(SDL_Window* window, int pixel_size) : window(window), pixel_size(pixel_size) {}

	void clear() const;
	void draw(const vec2& position, SDL_Surface* surface, const SDL_Rect& rect) const;
	void draw(const vec2& position, const sprite& sprite) const;
};

/* Singleton responsible for loading and caching textures to 
 * ensure they aren't loaded more than once. */
class texture_manager {
private:
	std::unordered_map<std::string, SDL_Surface*> cache;
public:
	static inline texture_manager& instance() {
		static texture_manager i;
		return i;
	}

	~texture_manager();

	static SDL_Surface* load(const char* filename);
};
