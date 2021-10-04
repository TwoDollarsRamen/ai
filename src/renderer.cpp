#include "stb_image.h"

#include "renderer.hpp"

void renderer::clear() const {
	SDL_Surface* backbuffer = SDL_GetWindowSurface(window);

	/* Clear the screen by filling the backbuffer with a black rectangle. */
	SDL_Rect screen_rect = { 0 };
	SDL_GetWindowSize(window, &screen_rect.w, &screen_rect.h);
	SDL_FillRect(backbuffer, &screen_rect, 0x0);
}

void renderer::draw(const vec2& position, SDL_Surface* surface, const SDL_Rect& rect) const {	
	SDL_Surface* backbuffer = SDL_GetWindowSurface(window);

	/* The destination position, width and height are multiplied
	 * by the pixel size, because this game uses pixel art. */
	SDL_Rect dst = {
		(i32)position.x * pixel_size,
		(i32)position.y * pixel_size,
		rect.w * pixel_size,
		rect.h * pixel_size
	};

	SDL_BlitScaled(surface, &rect, backbuffer, &dst);
}

void renderer::draw(const vec2& position, const sprite& sprite) const {
	draw(position, sprite.surface, sprite.rect);
}

SDL_Surface* texture_manager::load(const char* filename) {
	if (instance().cache.count(std::string(filename)) != 0) {
		/* No need to load the texture again; It is already in
		 * the cache, so we can simply return the cached version. */
		return instance().cache.at(std::string(filename));
	}

	/* Load and decompress the raw pixel data using STB image. */
	i32 width, height, component_count;
	u8* pixels = stbi_load(filename, &width, &height, &component_count, 0);
	if (!pixels) {
		fprintf(stderr, "Failed to load %s: %s.\n", filename, stbi_failure_reason());
		return nullptr;
	}

	/* Magic courtosy of:
	 *
	 * https://www.silbinarywolf.com/post/124379907558/loading-png-files-with-stb-image-and-sdl2 */
	i32 pitch;
	pitch = width * component_count;
	pitch = (pitch + 3) & ~3;
	i32 red_mask, green_mask, blue_mask, alpha_mask;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	red_mask =   0x000000FF;
	green_mask = 0x0000FF00;
	blue_mask =  0x00FF0000;
	alpha_mask = (component_count == 4) ? 0xFF000000 : 0;
#else
	i32 s = (component_count == 4) ? 0 : 8;
	red_mask =   0xFF000000 >> s;
	green_mask = 0x00FF0000 >> s;
	blue_mask =  0x0000FF00 >> s;
	alpha_mask = 0x000000FF >> s;
#endif

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height,
			component_count * 8, pitch, red_mask, green_mask,
			blue_mask, alpha_mask);

	if (!surface) {
		fprintf(stderr, "Failed to create surface from pixel data loaded from: %s: %s\n",
				filename, SDL_GetError());
		stbi_image_free(pixels);
		return nullptr;
	}

	stbi_image_free(pixels);

	return surface;
}

texture_manager::~texture_manager() {
	for (const auto& p : instance().cache) {
		SDL_FreeSurface(p.second);
	}
}
