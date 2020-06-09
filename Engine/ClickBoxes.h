#pragma once

#include "Vec2.h"

struct CBox
{
	CBox() = default;
	CBox(const Vei2& p, const Vei2& P)
		:
		p0(p),
		p1(P)
	{}
	CBox(const CBox& other) = delete;
	CBox operator=(const CBox& other) = delete;
	~CBox() = delete;
	Vei2 p0;
	Vei2 p1;
};