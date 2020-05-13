#pragma once

#include "Grid.h"

class Player
{
private:
	//Variables
	const Texture2D texture;
	const int dx = Graphics::ScreenWidth / 2 - texture.GetWidth() / 2;
	const int dy = Graphics::ScreenHeight / 2 - texture.GetHeight() / 2;
	Grid* cacheGrid;
	Vec2 position;
	Vec2 camera;
private:
	//Functions
public:
	//Variables
	float speed = 10.0f;
	const char playerRangeX = 10;
	const char playerRangeY = 10;
public:
	//Functions
	Player(Grid& grid, const int x);
	Player(const Player& other) = delete;
	Player operator=(const Player& other) = delete;
	~Player();
	void Draw(Graphics& gfx);
	void ChangePositionAndCam(const float _x, const float _y);
	Vec2 GetPosition() const;
	Vec2 GetCamera() const;
};