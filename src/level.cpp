#include <stdlib.h>
#include <string.h>

#include <fstream>

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
}

void level::draw(const renderer& ren) const {
	for (const auto& layer : layers) {
		for (u32 y = 0; y < layer.height; y++) {
			for (u32 x = 0; x < layer.width; x++) {
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

	u32 tileset_count;
	file.read(buf(tileset_count), 4);

	for (u32 i = 0; i < tileset_count; i++) {
		tileset current;

		/* Read the tileset name */
		u32 name_size;
		file.read(buf(name_size), 4);
		current.name = (char*)malloc(name_size + 1);
		file.read(current.name, name_size);
		current.name[name_size] = '\0';

		/* Read the tileset file path */
		u32 path_size;
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

	u32 layer_count;
 	file.read(buf(layer_count), 4);
	for (u32 i = 0; i < layer_count; i++) {
		tile_layer current;

		{
			u32 name_size;
			file.read(buf(name_size), 4);
			current.name = (char*)malloc(name_size + 1);
			file.read(current.name, name_size);
			current.name[name_size] = '\0';
		}

		/* -1 = unknown, 0 = tiles, 1 = objects */
		i32 layer_type;
		file.read(buf(layer_type), 4);

		switch (layer_type) {
		case 0: {
			file.read(buf(current.width), 4);
			file.read(buf(current.height), 4);

			current.tiles = (tile*)malloc(sizeof(tile) * current.width * current.height);

			for (u32 y = 0; y < current.height; y++) {
				for (u32 x = 0; x < current.width; x++) {
					i32 id;
					file.read(buf(id), 4);
					u32 tileset_idx = 0;
					if (id != -1) { /* -1 indicates an empty tile. */
						/* This code transforms the tile ID from something
						 * that is global to and ID that is relative to the
						 * index of the tileset that it belongs to. This makes
						 * it much faster to lookup the tile's tileset when
						 * it comes to rendering. */
						for (u32 ii = 0; ii < tilesets.size(); ii++) {
							const tileset& tileset = tilesets[ii];
							i32 tile_count = (tileset.image->w / tileset.tile_w) *
								(tileset.image->h / tileset.tile_h);

							if (id >= tile_count) {
								id -= tile_count;
								tileset_idx++;
							} else {
								break;
							}
						}
					}

					current.tiles[x + y * (u32)current.width] = tile {
							.id = id,
							.tileset_id = tileset_idx
						};
				}
			}
			break;
		}
		default:
			fprintf(stderr, "Unkown layer type: %u\n", layer_type);
			break;
		};

		layers.push_back(current);
	}

	file.close();

	return true;
}
