#include "Utilities.h"


// Contains simple vector operations.
namespace utilities
{
	vec2::vec2(std::initializer_list<float> l) 
	{
		x = *(l.begin());
		y = *(l.begin()+1);
	}

	vec2::vec2(float _x, float _y) : x{_x}, y{_y} 
	{}

	const vec2 vec2::operator/(float val)
	{
		return vec2(x/val, y/val);
	}

	const vec2 vec2::operator*(float val)
	{
		return vec2(x*val, y*val);
	}
	
	const vec2 operator*(float val, const vec2& v)
	{
		return vec2(v.x*val, v.y*val);
	}

	const vec2 vec2::operator-(float val)
	{
		return vec2(x-val, y-val);
	}

	const vec2 vec2::operator+(float val)
	{
		return vec2(x+val, y+val);
	}

	const vec2 vec2::operator-(const vec2& v)
	{
		return vec2(x-v.x, y-v.y);
	}

	const vec2 operator-(const vec2& v1, const vec2& v2)
	{
		return vec2(v1.x-v2.x, v1.y-v2.y);
	}

	const vec2 vec2::operator+(const vec2& v)
	{
		return vec2(x+v.x, y+v.y);
	}

	const vec2 operator+(const vec2& v1, const vec2& v2)
	{
		return vec2(v1.x+v2.x, v1.y+v2.y);
	}

	void vec2::vec2::operator=(const vec2& v)
	{
		x = v.x;
		y = v.y;
	}
	
	void vec2::operator+=(const vec2& v)
	{
		x = x + v.x;
		y = y + v.y;
	}

	const float dist(const float x1, const float y1, const float x2, const float y2)
	{
		return sqrt(pow(x1-x2,2) + pow(y1-y2,2) );
	}

	const float dist(const vec2& v1, const vec2& v2)
	{
		return dist(v1.x, v1.y, v2.x, v2.y);
	}

	const float length(const vec2& v)
	{
		return sqrt(pow(v.x,2) + pow(v.y,2) );
	}

	const vec2 normalize(const float x, const float y)
	{
		return length(vec2(x, y)) > 0.0 ? vec2(x, y) / length(vec2(x, y)) : vec2(0.0, 0.0);
	}

	const vec2 normalize(const vec2& v)
	{
		return normalize(v.x, v.y);
	}
};
