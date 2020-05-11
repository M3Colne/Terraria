#pragma once

#include "Graphics.h"

class Player
{
private:
	//Variables
	const Texture2D playerTexture;
private:
	//Functions
public:
	//Variables
	Vei2 position;
public:
	//Functions
	Player(const Vei2 pos);
	Player(const int x, const int y);
	void Draw(Graphics& gfx);
};