#pragma once

#include "Grid.h"

class Player
{
private:
	//Variables
	const Texture2D playerTexture;
	const int dx = Graphics::ScreenWidth / 2 - playerTexture.GetWidth() / 2;
	const int dy = Graphics::ScreenHeight / 2 - playerTexture.GetHeight() / 2;
	Vec2 position;
	Vec2 camera;
	float speed = 10.0f;
	const char playerRange = 10;
private:
	//Functions
public:
	//Variables
public:
	//Functions
	Player(const Grid& grid, const int x);
	void Draw(Graphics& gfx);
	void ChangePositionAndCam(const float _x, const float _y, const int W, const int H);
	Vec2 GetCamera() const;
};