#pragma once

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "common.hpp"
#include "stb_rect_pack.h"
#include "stb_truetype.h"

#define glyph_set_count 256

class renderer;

struct glyph_set {
	SDL_Surface* atlas;
	stbtt_bakedchar glyphs[256];

	~glyph_set();
};

class font {
private:
	unsigned char* data;
	stbtt_fontinfo info;
	glyph_set* sets[glyph_set_count];
	float size;
	int height;
public:
	font(const char* filename, float size);
	~font();

	glyph_set* load_glyph_set(int index);
	glyph_set* get_glyph_set(int codepoint);

	void draw_text(const renderer& renderer, vec2 position, const char* text);

	vec2 text_dimentions(const char* text);
};

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

	friend class font;
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
