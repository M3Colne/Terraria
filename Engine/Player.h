#pragma once

#include "Graphics.h"

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
private:
	//Functions
	void ChangePosition(const float _x, const float _y);
	void ChangePosition(const Vec2& v);
	void ChangePositionAndCam(const float _x, const float _y);
	void ChangePositionAndCam(const Vec2& v);
	void PlayerFixToWorld(const int W, const int H);
	void CameraFixToWorld(const int W, const int H);
public:
	//Variables
public:
	//Functions
	Player(const float x, const float y);
	void Draw(Graphics& gfx);
	void Update(bool wP, bool sP, bool aP, bool dP, const int W, const int H, float dt);
	Vec2 GetCamera() const;
};