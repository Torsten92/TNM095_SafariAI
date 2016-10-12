#ifndef _UTILITIES_
#define _UTILITIES_

#include "Globals.h"

#include <initializer_list>
#include <cmath>


// Contains simple 2D-vector operations.
namespace utilities
{
	struct vec2
	{
		vec2(std::initializer_list<float> l);
		vec2(float _x, float _y);
		const vec2 operator/(float val);
		const vec2 operator*(float val);
		const vec2 operator-(float val);
		const vec2 operator+(float val);
		const vec2 operator-(const vec2& v);
		const vec2 operator+(const vec2& v);
		void operator=(const vec2& v);
		void operator+=(const vec2& v);
		friend const vec2 operator*(float val, const vec2& v);
		friend const vec2 operator/(const vec2& v, float val);
		friend const vec2 operator-(const vec2& v1, const vec2& v2);
		friend const vec2 operator+(const vec2& v1, const vec2& v2);
		
		float x, y;
	};

	const float dist(const float x1, const float y1, const float x2, const float y2);
	const float dist(const vec2& v1, const vec2& v2);
	const float length(const vec2& v);
	const vec2 normalize(const float x, const float y);
	const vec2 normalize(const vec2& v);
	const float dot(const vec2& v1, const vec2& v2);
	const float angle_between(const vec2& v1, const vec2& v2);
	const vec2 rotate(const vec2& v, float angle);

	std::string convert_type(int type);
	std::string convert_action(int action);
};

#endif