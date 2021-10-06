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
public:
	/* Pathfinding stuff */
	struct node {
		bool obstacle = false;
		bool visited = false;
		float global_goal;
		float local_goal;
		i32 x, y;
		std::vector<node*> neighbours;
		node* parent = nullptr;
	};

private:
	std::vector<tileset> tilesets;
	std::vector<tile_layer> layers;

	node* nodes = nullptr;
	i32 map_width, map_height;
public:	
	~level();

	void draw(const renderer& ren) const;
	bool load(const char* filename);

	std::vector<vec2> find_path(i32 start_x, i32 start_y, i32 end_x, i32 end_y);
};
