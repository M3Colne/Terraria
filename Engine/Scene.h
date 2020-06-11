#pragma once

#include "CBox.h"
#include "Texture2D.h"

struct Scene
{
	Texture2D texture;
	CBox* boxes = nullptr;
	int nBoxes = 0;
	bool isShown = false;
};