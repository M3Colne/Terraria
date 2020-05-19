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
	Vec2 velocity;
	Vec2 acceleration;
	const float maxSpeed = 100.0f;
	const float defaultAcc = 85.0f;
	const float defaultDeacc = 90.0f; //Bigger than the acceleartion
	const float maxAcceleration = 85.0f;
	const float gravity = 35.0f;
	const float frictionForce = 85.0f;
	const char playerRangeX = 10;
	const char playerRangeY = 10;
private:
	//Functions
	int sgn(const float d) const; //Wow, I'm surprised C++ doesn't have this function
	void ApplyForce(const float _x, const float _y);
	void ApplyForce(Vec2 f);
	void StopX();
	void StopY();
	void Fix(const float dt);
	float SweptAABB(const float dt) const;
	bool BroadphasingCollision() const;
public:
	//Variables
public:
	//Functions
	Player(Grid& grid, const int x);
	Player(const Player& other) = delete;
	Player operator=(const Player& other) = delete;
	~Player();
	void Draw(Graphics& gfx);
	void Update(Keyboard& kbd, Mouse& micky, const float dt);
	Vec2 GetPosition() const;
	Vec2 GetCamera() const;
};