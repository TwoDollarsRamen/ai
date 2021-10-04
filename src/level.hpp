#pragma once

#include <vector>

#include "common.hpp"
#include "renderer.hpp"

struct tileset {
	SDL_Surface* image = nullptr;
	char* name = nullptr;
	i32 tile_w, tile_h;
};

struct tile {
	i32 id;
	u32 tileset_id;
};

struct tile_layer {
	tile* tiles = nullptr;
	char* name = nullptr;
	i32 width, height;
};

class level {
private:
	std::vector<tileset> tilesets;
	std::vector<tile_layer> layers;

public:
	~level();

	void draw(const renderer& ren) const;
	bool load(const char* filename);
};
