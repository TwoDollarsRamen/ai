#include <iterator>
#include <algorithm>

#include "common.hpp"

bool rect_overlap(const SDL_Rect& a, const SDL_Rect& b, vec2& normal) {
	if (!(
		a.x + a.w > b.x &&
		a.y + a.h > b.y &&
		a.x < b.x + b.w &&
		a.y < b.y + b.h)) {
		normal = vec2(0, 0);
		return false;
	}

	/* Calculate the amount of overlap from the top, bottom, left and right.
	 *
	 * This information is then used to calculate the collision normal. */
	int right  = (a.x + a.w) - b.x;
	int left   = (b.x + b.w) - a.x;
	int top    = (b.y + b.h) - a.y;
	int bottom = (a.y + a.h) - b.y;

	int overlap[] = { right, left, top, bottom };

	/* Sort so that the first value is the smallest. */
	std::sort(std::begin(overlap), std::end(overlap));

	if (overlap[0] == abs(right)) {
		normal.x = 1.0f;
	} else if (overlap[0] == abs(left)) {
		normal.x = -1.0f;
	} else if (overlap[0] == abs(bottom)) {
		normal.y = 1.0f;
	} else if (overlap[0] == abs(top)) {
		normal.y = -1.0f;
	} 

	return true;
}
