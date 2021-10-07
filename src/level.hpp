#pragma once

#include <vector>

#include "common.hpp"
#include "renderer.hpp"

struct tileset {
	SDL_Surface* image = nullptr;
	char* name = nullptr;
	int tile_w, tile_h;
};

struct tile {
	int id;
	int tileset_id;
};

struct tile_layer {
	tile* tiles = nullptr;
	char* name = nullptr;
	int width, height;
};

class level {
public:
	/* Pathfinding stuff */
	struct node {
		bool obstacle = false;
		bool visited = false;
		float global_goal;
		float local_goal;
		int x, y;
		std::vector<node*> neighbours;
		node* parent = nullptr;
	};

private:
	std::vector<tileset> tilesets;
	std::vector<tile_layer> layers;

	node* nodes = nullptr;
	int map_width, map_height;
public:	
	~level();

	void draw(const renderer& ren) const;
	bool load(const char* filename);

	std::vector<vec2> find_path(int start_x, int start_y, int end_x, int end_y);
};
