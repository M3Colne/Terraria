#pragma once

#include "Graphics.h"

class Player
{
private:
	//Variables
	const Texture2D playerTexture;
	Vec2 position;
	float speed = 15.0f;

	//Constants
	const int dx = Graphics::ScreenWidth / 2 - playerTexture.GetWidth() / 2;
	const int dy = Graphics::ScreenHeight / 2 - playerTexture.GetHeight() / 2;
private:
	//Functions
public:
	//Variables
public:
	//Functions
	Player(const float x, const float y);
	void Draw(Graphics& gfx);
	void Update(bool wP, bool sP, bool aP, bool dP, float dt);
	Vec2 GetCamera() const;
};