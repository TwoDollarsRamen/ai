#pragma once

#include <stdint.h>

/* Being able to specify the size of integers
 * works nicely for binary serialisation;
 *
 * Shorter to type than for example `uint32_t'
 * or `unsigned int'. */
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

struct vec2 {
	float x, y;

	vec2(float x) : x(x), y(x) {}
	vec2(float x, float y) : x(x), y(y) {}
};
