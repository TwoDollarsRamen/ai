#pragma once

#include <vector>
#include <functional>

#include "common.hpp"
#include "renderer.hpp"

class world;

struct menu_item {
	std::string name;
	std::function<void(world& world)> on_select;
};

/* For creating menus that are navigatable
 * by way of the arrow keys. */
class menu {
private:
	world& w;
	font& f;
	sprite selector;

	std::vector<menu_item> items;

	int selected_item = 0;
public:
	menu(world& w, font& f, sprite selector);

	void add_item(const char* name, std::function<void(world& world)> on_select);

	void handle_events(const SDL_Event& e);

	void draw(const renderer& ren);
};
