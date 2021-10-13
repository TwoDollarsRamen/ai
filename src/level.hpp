#pragma once

#include <vector>

#include "common.hpp"
#include "renderer.hpp"
#include "agent.hpp"

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
	const sprite door_sprite;
	const sprite key_sprite;

	std::vector<SDL_Rect> collisions;
	std::vector<agent> agents;

	std::vector<SDL_Rect> doors;
	std::vector<SDL_Rect> keys;

	level();
	~level();

	void draw(const renderer& ren) const;
	bool load(const char* filename);

	std::vector<vec2> find_path(int start_x, int start_y, int end_x, int end_y);

	vec2 get_random_node();

	/* Generic functions to resolve collisions with a body. Modifies the body's
	 * position should a collision occur.*/
	void resolve_collisions_with_body(const SDL_Rect& body_collider, vec2& body_position) const;
};
