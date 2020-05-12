#include "Player.h"

void Player::ChangePositionAndCam(const float _x, const float _y, const int W, const int H)
{
	//Update the player position
	position.x += _x;
	position.y += _y;
	//Fix player to world
	if (position.x < 0)
	{
		position.x = 0;
	}
	else if (position.x + playerTexture.GetWidth() > W)
	{
		position.x = float(W - playerTexture.GetWidth());
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	else if (position.y + playerTexture.GetHeight() > H)
	{
		position.y = float(H - playerTexture.GetHeight());
	}

	//Fix camera to world
	if (position.x <= dx)
	{
		camera.x = 0.0f;
	}
	else if (position.x >= W - playerTexture.GetWidth() - dx)
	{
		camera.x = float(W - Graphics::ScreenWidth);
	}
	else
	{
		camera.x += _x;
	}

	if (position.y <= dy)
	{
		camera.y = 0.0f;
	}
	else if (position.y >= H - playerTexture.GetHeight() - dy)
	{
		camera.y = float(H - Graphics::ScreenHeight);
	}
	else
	{
		camera.y += _y;
	}
}

Player::Player(const float x, const float y)
	:
	playerTexture("./Assets/playerTextureSheet20x40.bmp"),
	position(x, y),
	camera(position.x - float(dx), position.y - float(dy))
{
}

void Player::Draw(Graphics& gfx)
{
	gfx.DrawTextureChroma(int(position.x - camera.x), int(position.y - camera.y),
		0, 0, playerTexture.GetWidth(), playerTexture.GetHeight(), playerTexture, Colors::Magenta);
}

void Player::Update(bool wP, bool sP, bool aP, bool dP, const int W, const int H, float dt)
{
	if (wP)
	{
		ChangePositionAndCam(0.0f, -speed * dt, W, H);
	}
	if (sP)
	{
		ChangePositionAndCam(0.0f, speed * dt, W, H);
	}
	if (aP)
	{
		ChangePositionAndCam(-speed * dt, 0.0f, W, H);
	}
	if (dP)
	{
		ChangePositionAndCam(speed * dt, 0.0f, W, H);
	}
}

Vec2 Player::GetCamera() const
{
	return camera;
}