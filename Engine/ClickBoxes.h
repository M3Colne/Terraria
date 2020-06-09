#pragma once

#include "Vec2.h"

struct CBox
{
	CBox(const Vei2& p, const Vei2& P)
		:
		p0(p),
		p1(P)
	{}
	Vei2 p0;
	Vei2 p1;
};