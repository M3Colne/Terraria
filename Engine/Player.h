#pragma once

#include "Grid.h"
#include "Keyboard.h"
#include "Mouse.h"

class Player
{
private:
	//Variables
	const Texture2D texture;
	Grid* cacheGrid;
	Vec2 position;
	Vec2 velocity;
	Vec2 acceleration;
	const float maxSpeedX = 100.0f;
	const float maxSpeedY = 800.0f;
	const float defaultAcc = 90.0f;
	const float defaultDeacc = 135.0f; //Bigger than the acceleartion
	const float maxAcceleration = 150.0f;
	const float gravity = 200.0f;
	const float frictionForce = 85.0f;
	const char playerRangeX = 10;
	const char playerRangeY = 10;
	const float initialJumpImpulse = 100.0f;
	unsigned framesInAir = 0;
	bool onGround = true;
private:
	//Functions
	int sgn(const float d) const; //Wow, I'm surprised C++ doesn't have this function
	void ApplyForce(const float _x, const float _y);
	void ApplyForce(Vec2 f);
	void StopX();
	void StopY();
	void Collisions(bool& COLL, const float dt);
	float SweptAABB(const int id, Vec2& n, const float dt) const;
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
	int GetCameraX() const;
	int GetCameraY() const;
};