#pragma once

#include "Grid.h"
#include "Keyboard.h"
#include "Mouse.h"

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
	Vec2 speed;
	Vec2 acceleration;
	const float defaultAcc = 1.0f;
	const float maxSpeed = 10.0f;
	const float maxAcceleration = 5.0f;
	const float gravity = 1.0f;
	const char playerRangeX = 10;
	const char playerRangeY = 10;
private:
	//Functions
	void ApplyForce(const float _x, const float _y);
	void ApplyForce(Vec2 f);
	void Move(const float dt);
	void Physics(const float dt);
	void MaxAcc();
	void MaxSpeed();
public:
	//Variables
public:
	//Functions
	Player(Grid& grid, const int x);
	Player(const Player& other) = delete;
	Player operator=(const Player& other) = delete;
	~Player();
	void Draw(Graphics& gfx);
	void Update(Keyboard& kbd, Mouse& micky, float dt);
	Vec2 GetPosition() const;
	Vec2 GetCamera() const;
};