#pragma once

#include "Vec2.h"

struct CBox
{
    bool isCollidingWithMouse(Vei2 mVector)
    {
        isCollidingWithMouse(mVector.x, mVector.y);
    }
    bool isCollidingWithMouse(int x, int y)
    {
        return x >= p0.x && x <= p1.x &&
            y >= p0.y && y <= p1.y;
    }
    Vei2 p0 = { 400, 300 };
    Vei2 p1 = { 425, 300 };
};