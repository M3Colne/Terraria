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
private:
	//Functions
public:
	//Variables
public:
	//Functions
	Player(const float x, const float y);
	void Draw(Graphics& gfx);
	Vec2 GetCamera() const;
};