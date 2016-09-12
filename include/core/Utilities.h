#ifndef _UTILITIES_
#define _UTILITIES_


#ifdef __unix__
	#include <stdio.h>
	#include <unistd.h>
	#include <stdlib.h>
	#include <random>
#elif defined(_WIN32) || defined(WIN32)
	#define OS_Windows
	#include <windows.h>
	#include <stdio.h>
	#include <initializer_list>
	#include <cmath>
#endif

// Contains simple 2D-vector operations.
namespace utilities
{
	#ifndef OS_Windows
		#include <initializer_list>
		#include <cmath>
	#endif
	struct vec2
	{
		vec2(std::initializer_list<float> l);
		vec2(float _x, float _y);
		vec2 operator/(float val);
		vec2 operator*(float val);
		friend const vec2 operator*(float val, const vec2& v);
		float x, y;
	};

	const float dist(const float x1, const float y1, const float x2, const float y2);
	const float dist(const vec2& v1, const vec2& v2);
	const float length(const vec2& v);
	const vec2 normalize(const float x, const float y);
	const vec2 normalize(const vec2& v);

};

#endif