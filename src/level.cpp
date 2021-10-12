#include <stdlib.h>
#include <string.h>

#include <cmath>
#include <fstream>
#include <list>

#include "level.hpp"

#define buf(v_) (char*)&(v_)

level::~level() {
	for (const auto& layer : layers) {
		free(layer.tiles);
		free(layer.name);
	}

	for (const auto& tileset : tilesets) {
		free(tileset.name);
	}

	delete[] nodes;
}

void level::draw(const renderer& ren) const {
	for (const auto& layer : layers) {
		for (int y = 0; y < layer.height; y++) {
			for (int x = 0; x < layer.width; x++) {
				tile t = layer.tiles[x + y * layer.width];
				if (t.id != -1) {
					const tileset& set = tilesets[t.tileset_id];
					SDL_Surface* surf = set.image;

					SDL_Rect sub = {
						.x = (t.id % (surf->w / set.tile_w)) * set.tile_w,
						.y = (t.id / (surf->w / set.tile_h)) * set.tile_h,
						.w = set.tile_w,
						.h = set.tile_h
					};
					ren.draw(vec2(x * set.tile_w, y * set.tile_h),
							surf, sub);
				}
			}
		}
	}
}

bool level::load(const char* filename) {
	std::fstream file;
	file.open(filename, std::ios_base::in | std::ios_base::binary);

	if (!file.good()) {
		fprintf(stderr, "Failed to load map: %s: Couldn't open file\n", filename);
		return false;
	}

	int tileset_count;
	file.read(buf(tileset_count), 4);

	for (int i = 0; i < tileset_count; i++) {
		tileset current;

		/* Read the tileset name */
		int name_size;
		file.read(buf(name_size), 4);
		current.name = (char*)malloc(name_size + 1);
		file.read(current.name, name_size);
		current.name[name_size] = '\0';

		/* Read the tileset file path */
		int path_size;
		file.read(buf(path_size), 4);

		char* path = (char*)malloc(path_size + 1);
		file.read(path, path_size);
		path[path_size] = '\0';

		current.image = texture_manager::load(path);

		free(path);

		file.read(buf(current.tile_w), 4);
		file.read(buf(current.tile_h), 4);

		tilesets.push_back(current);
	}

	int layer_count;
 	file.read(buf(layer_count), 4);
	for (int i = 0; i < layer_count; i++) {
		tile_layer current;

		/* Keeps track of whether or not the layer should be pushed back into the vector.
		 * The layer vector only stores the layers for rendering. */
		bool want_storage = true; 

		{
			int name_size;
			file.read(buf(name_size), 4);
			current.name = (char*)malloc(name_size + 1);
			file.read(current.name, name_size);
			current.name[name_size] = '\0';
		}

		/* -1 = unknown, 0 = tiles, 1 = objects */
		int layer_type;
		file.read(buf(layer_type), 4);

		switch (layer_type) {
		case 0: {
			if (strcmp(current.name, "agent mask") != 0) {
				file.read(buf(current.width), 4);
				file.read(buf(current.height), 4);

				current.tiles = (tile*)malloc(sizeof(tile) * current.width * current.height);

				for (int y = 0; y < current.height; y++) {
					for (int x = 0; x < current.width; x++) {
						int id;
						file.read(buf(id), 4);
						int tileset_idx = 0;
						if (id != -1) { /* -1 indicates an empty tile. */
							/* This code transforms the tile ID from something
							 * that is global to and ID that is relative to the
							 * index of the tileset that it belongs to. This makes
							 * it much faster to lookup the tile's tileset when
							 * it comes to rendering. */
							for (int ii = 0; ii < tilesets.size(); ii++) {
								const tileset& tileset = tilesets[ii];
								int tile_count = (tileset.image->w / tileset.tile_w) *
									(tileset.image->h / tileset.tile_h);

								if (id >= tile_count) {
									id -= tile_count;
									tileset_idx++;
								} else {
									break;
								}
							}
						}

						current.tiles[x + y * (int)current.width] = tile {
								.id = id,
								.tileset_id = tileset_idx
							};
					}
				}
			} else {
				/* Load the pathfinding nodes from the tile layer named
				 * `agent mask'. Where there is a tile, the agents can't walk. */
				want_storage = false;

				file.read(buf(map_width), 4);
				file.read(buf(map_height), 4);

				nodes = new node[map_width * map_height];

				for (int y = 0; y < map_height; y++) {
					for (int x = 0; x < map_width; x++) {
						int tile_id;
						file.read(buf(tile_id), 4);

						nodes[x + y * map_width] = node {
							.obstacle = tile_id != -1,
							.visited = false,
							.global_goal = 0.0f,
							.local_goal = 0.0f,
							.x = x, .y = y,
							.neighbours = {},
							.parent = nullptr
						};
					}
				}

				/* Link up the pathfinding nodes with their neighbours */
				for (int y = 0; y < map_height; y++) {
					for (int x = 0; x < map_width; x++) {
						node* node = nodes + (x + y * map_width);

						if (y > 0) {
							node->neighbours.push_back(
									&nodes[x + (y - 1) * map_width]);
						}
						if (y < map_height - 1) {
							node->neighbours.push_back(
									&nodes[x + (y + 1) * map_width]);
						}
						if (x > 0) {
							node->neighbours.push_back(
									&nodes[(x - 1) + y * map_width]);
						}
						if (x < map_width - 1) {
							node->neighbours.push_back(
									&nodes[(x + 1) + y * map_width]);
						}
					}
				}
			}
			break;
		}
		case 1: {
			want_storage = false;

			int object_count;
			file.read(buf(object_count), 4);

			SDL_Surface* agent_atlas = texture_manager::load("res/atlas.png");

			for (int ii = 0; ii < object_count; ii++) {
				SDL_Rect rect;

				file.read(buf(rect.x), 4);
				file.read(buf(rect.y), 4);
				file.read(buf(rect.w), 4);
				file.read(buf(rect.h), 4);

				if (strcmp("collisions", current.name) == 0) {
					collisions.push_back(rect);
				} else if (strcmp("agents", current.name) == 0) {
					agent a(agent_atlas);

					a.position = vec2(rect.x, rect.y);

					agents.push_back(a);
				}
			}
			break;
		}
		default:
			want_storage = false;

			fprintf(stderr, "Unkown layer type: %u\n", layer_type);
			break;
		};

		if (want_storage) {
			layers.push_back(current);
		} else {
			free(current.name);
		}
	}

	file.close();

	return true;
}

std::vector<vec2> level::find_path(int start_x, int start_y, int end_x, int end_y) {
	if (!(start_x < map_width && start_y < map_height && start_x >= 0 && start_y >= 0)) {
		return std::vector<vec2>();
	}

	/* Reset the navigation graph */
	for (int y = 0; y < map_height; y++) {
		for (int x = 0; x < map_width; x++) {
			nodes[x + y * map_width].visited = false;
			nodes[x + y * map_width].global_goal = std::numeric_limits<float>::max();
			nodes[x + y * map_width].local_goal = std::numeric_limits<float>::max();
			nodes[x + y * map_width].parent = nullptr;
		}
	}

	auto distance = [](node* a, node* b) -> float {
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto start = nodes + (start_x + start_y * map_width);
	auto end   = nodes + (end_x + end_y * map_width);

	start->local_goal = 0.0f;
	start->global_goal = distance(start, end);

	auto current = start;

	std::list<node*> to_test;
	to_test.push_back(start);

	while (!to_test.empty()) {
		/* Sort the untested nodes so that the node with the lowest goal comes first. */
		to_test.sort([](const node* a, const node* b) -> bool {
					return a->global_goal < b->global_goal;
				});

		current = to_test.front();

		if (current == end) {
			break;
		}

		current->visited = true;
		to_test.pop_front();

		for (auto neighbour : current->neighbours) {
			if (!neighbour->visited && !neighbour->obstacle) {
				float possible_local_goal = current->local_goal + 1.0f;
				float possible_global_goal = possible_local_goal + distance(neighbour, end);

				if (std::find(to_test.begin(), to_test.end(), neighbour) == to_test.end()) {
					neighbour->parent = current;
					neighbour->local_goal = possible_local_goal;
					neighbour->global_goal = possible_global_goal;

					to_test.push_back(neighbour);
				} else if (possible_global_goal < neighbour->global_goal) {
					neighbour->parent = current;
					neighbour->local_goal = possible_local_goal;
					neighbour->global_goal = possible_global_goal;
				}
			}
		}
	}

	std::vector<vec2> r;
	auto p = end;
	while (p) {
		r.push_back(vec2(p->x, p->y));
		p = p->parent;
	}

	/* Since the first value in the vector is the end node, the vector
	 * must be reversed. */
	std::reverse(r.begin(), r.end());

	return r;
}

vec2 level::get_random_node() {
	const int min = 0;
	const int max = map_width * map_height;

	const node* node;
	do {
		node = nodes + ((rand() % (max - min + 1)) + min);
	} while (node->obstacle);

	return vec2(node->x * grid_size, node->y * grid_size);
}
