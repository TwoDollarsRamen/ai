#include <fstream>

#include "stb_image.h"

#include "renderer.hpp"

/* Converts a UTF-8 character to a codepoint.
 * 
 * Based on the information found in this stack overflow answer:
 * https://stackoverflow.com/questions/6240055/manually-converting-unicode-codepoints-into-utf-8-and-utf-16
 *
 * Intended to be used with a string, returns a pointer
 * to the next character in the string. Places the
 * result into `dst'.*/
static const char* utf8_to_codepoint(const char* text, unsigned int* dst) {
	unsigned int res, n;
	switch (*text & 0xf0) {
		case 0xf0 : res = *text & 0x07; n = 3; break;
		case 0xe0 : res = *text & 0x0f; n = 2; break;
		case 0xd0 :
		case 0xc0 : res = *text & 0x1f; n = 1; break;
		default   : res = *text;        n = 0; break;
	}
	while (n--) {
		res = (res << 6) | (*(++text) & 0x3f);
	}
	*dst = res;
	return text + 1;
}

glyph_set::~glyph_set() {
	SDL_FreeSurface(atlas);
}

font::font(const char* filename, float size) : size(size) {
	for (int i = 0; i < glyph_set_count; i++) {
		sets[i] = nullptr;
	}

	std::fstream file;
	file.open(filename, std::ios_base::in | std::ios_base::binary);

	if (!file.good()) {
		fprintf(stderr, "Failed to open font file: %s\n", filename);
		return;
	}

	file.seekg(0, std::ios::end);
	unsigned int file_size = file.tellg();
	file.seekg(0, std::ios::beg);

	data = (unsigned char*)malloc(file_size);

	file.read((char*)data, file_size);

	int r = stbtt_InitFont(&info, data, 0);
	if (!r) {
		free(data);
		fprintf(stderr, "Failed to load font: %s\n", filename);
		return;
	}

	/* Calculate the height of the font */
	int ascent, descent, linegap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &linegap);
	float scale = stbtt_ScaleForMappingEmToPixels(&info, size);
	height = (ascent - descent + linegap) * scale + 0.5;

	/* Make the tab and newline characters invisible
	 * by making their rectangle widths equal to zero.
	 *
	 * This is because some fonts add a visible newline
	 * or tab character */
	auto glyphs = get_glyph_set('\n')->glyphs;
	glyphs['\t'].x1 = glyphs['\t'].x0;
	glyphs['\n'].x1 = glyphs['\n'].x0;
}

font::~font() {
	free(data);
}

glyph_set* font::load_glyph_set(int index) {
	auto set = new glyph_set();

	int width = 128;
	int height = 128;
	int r;

	unsigned char* pixels = nullptr;

	for (;;) {
		pixels = (unsigned char*)malloc(width * height);

		float scale = stbtt_ScaleForMappingEmToPixels(&info, 1) /
				stbtt_ScaleForPixelHeight(&info, 1);
		r = stbtt_BakeFontBitmap((unsigned char*)data, 0, size * scale,
				pixels, width, height, index * 256, 256, set->glyphs);

		/* The bitmap was not large enough to pack all the glyphs,
		 * increase the size and try again. */
		if (r <= 0) {
			width *= 2;
			height *= 2;

			free(pixels);

			continue;
		}

		break;
	}

	/* Calculate the y axis offsets and the width of each character. */
	int ascent, descent, linegap;
	stbtt_GetFontVMetrics(&info, &ascent, &descent, &linegap);
	float scale = stbtt_ScaleForMappingEmToPixels(&info, size);
	int scaled_ascent = ascent * scale + 0.5;
	for (int i = 0; i < 256; i++) {
		set->glyphs[i].yoff += scaled_ascent;
		set->glyphs[i].xadvance = floor(set->glyphs[i].xadvance);
	}

	/* The bitmap created by stb_truetype only contains one channel,
	 * but SDL expects four, so it has to be converted. */
	 unsigned char* image = (unsigned char*)malloc(width * height * 4);
	 for (int src = 0, dst = 0; src < width * height; dst += 4, src++) {
		auto a = pixels[src];

		image[dst + 0] = 255;
		image[dst + 1] = 255;
		image[dst + 2] = 255;
		image[dst + 3] = a;
	 }
	 free(pixels);

	set->atlas = SDL_CreateRGBSurfaceWithFormatFrom((void*)image, width, height,
		32, 4 * width, SDL_PIXELFORMAT_RGBA32);

	return set;
}

glyph_set* font::get_glyph_set(int codepoint) {
	int index = (codepoint >> 8) % glyph_set_count;

	if (!sets[index]) {
		sets[index] = load_glyph_set(index);
	}

	return sets[index];
}

vec2 font::text_dimentions(const char* text) {
	return vec2(0, 0);
}

void font::draw_text(const renderer& ren, vec2 position, const char* text) {
	SDL_Surface* backbuffer = SDL_GetWindowSurface(ren.window);

	while (*text) {
		unsigned int codepoint = 0;

		text = utf8_to_codepoint(text, &codepoint);

		glyph_set* set = get_glyph_set(codepoint);
		auto glyph = &set->glyphs[codepoint & 0xff];

		SDL_Rect src = { glyph->x0, glyph->y0, glyph->x1 - glyph->x0, glyph->y1 - glyph->y0 };
		SDL_Rect dst = {
			(int)position.x + glyph->xoff,
			(int)position.y + glyph->yoff,
			src.w, src.h
		};

		SDL_BlitSurface(set->atlas, &src, backbuffer, &dst);

		position.x += glyph->xadvance;
	}
}

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
		(int)position.x * pixel_size,
		(int)position.y * pixel_size,
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
	int width, height, component_count;
	unsigned char* pixels = stbi_load(filename, &width, &height, &component_count, 0);
	if (!pixels) {
		fprintf(stderr, "Failed to load %s: %s.\n", filename, stbi_failure_reason());
		return nullptr;
	}

	/* Magic courtosy of:
	 *
	 * https://www.silbinarywolf.com/post/124379907558/loading-png-files-with-stb-image-and-sdl2
	 *
	 * What this code does is set up the masks of where different pixels are
	 * located in the raw data depending on how SDL expects them. */
	int pitch;
	pitch = width * component_count;
	pitch = (pitch + 3) & ~3;
	int red_mask, green_mask, blue_mask, alpha_mask;

#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	red_mask =   0x000000FF;
	green_mask = 0x0000FF00;
	blue_mask =  0x00FF0000;
	alpha_mask = (component_count == 4) ? 0xFF000000 : 0;
#else
	int s = (component_count == 4) ? 0 : 8;
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

	instance().cache[std::string(filename)] = surface;

	return surface;
}

texture_manager::~texture_manager() {
	for (const auto& p : instance().cache) {
		SDL_FreeSurface(p.second);
	}
}
