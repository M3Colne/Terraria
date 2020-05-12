#include "Player.h"

void Player::ChangePositionAndCam(const float _x, const float _y)
{
	//Update the player position
	position.x += _x;
	position.y += _y;
	//Update the camera by the player position
	camera.x += _x;
	camera.y += _y;
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

void Player::Update(bool wP, bool sP, bool aP, bool dP, float dt)
{
	if (wP)
	{
		ChangePositionAndCam(0.0f, -speed * dt);
	}
	if (sP)
	{
		ChangePositionAndCam(0.0f, speed * dt);
	}
	if (aP)
	{
		ChangePositionAndCam(-speed * dt, 0.0f);
	}
	if (dP)
	{
		ChangePositionAndCam(speed * dt, 0.0f);
	}
}

Vec2 Player::GetCamera() const
{
	return camera;
}