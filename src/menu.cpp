#include "menu.hpp"

menu::menu(world& w, font& f, sprite selector) : w(w), f(f), selector(selector) {}

void menu::add_item(const char* name, std::function<void(world& world)> on_select) {
	items.push_back(menu_item { name, on_select });
}

void menu::handle_events(const SDL_Event& e) {
	switch (e.type) {
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym) {
		case SDLK_UP:     selected_item--; break;
		case SDLK_DOWN:   selected_item++; break;
		case SDLK_RETURN: items[selected_item].on_select(w); break;
		}
		break;
	};

	if (selected_item < 0) {
		selected_item = items.size() - 1;
	}

	if (selected_item > items.size() - 1) {
		selected_item = 0;
	}
}

void menu::draw(const renderer& ren) {
	SDL_Surface* backbuffer = SDL_GetWindowSurface(ren.window);

	float y = 100;

	for (int i = 0; i < items.size(); i++) {
		const auto& item = items[i];

		auto dim = f.text_dimentions(item.name.c_str());

		float x = (backbuffer->w / 2) - (dim.x / 2);

		/* Draw the text. */
		f.draw_text(ren, vec2(x, y), item.name.c_str());
	
		if (selected_item == i) { /* Draw the selector. */
			/* The selector is positioned to the left of the current piece of text. */
			ren.draw(vec2(
				(x - selector.rect.w * 2) / ren.pixel_size,
				(y + (dim.y / 2) - (selector.rect.h / 2)) / ren.pixel_size),
				selector, true);
		}

		y += dim.y;	
	}
}
