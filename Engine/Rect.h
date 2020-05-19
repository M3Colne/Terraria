#pragma once

#include "Vec2.h"

struct Rect
{
	int left;
	int right;
	int top;
	int bottom;

	Rect(Vec2 pos, int w, int h)
		:
		left(int(pos.x)),
		right(int(pos.x) + w),
		top(int(pos.y)),
		bottom(int(pos.y) + h)
	{
	}
	Rect(Vec2 p0, Vec2 p1)
		:
		left(int(p0.x)),
		right(int(p1.x)),
		top(int(p0.y)),
		bottom(int(p1.y))
	{
	}
	int GetWidth() const
	{
		return right - left;
	}
	int GetHeight() const
	{
		return bottom - top;
	}
	static bool Colliding(const Rect b0, const Rect b1)
	{
		return b0.right > b1.left &&
			b0.left < b1.right &&
			b0.bottom > b1.top &&
			b0.top < b1.bottom;
	}
};