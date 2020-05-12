#include "Player.h"

void Player::ChangePosition(const float _x, const float _y)
{
	//Update the player position
	position.x += _x;
	position.y += _y;
}

void Player::ChangePosition(const Vec2& v)
{
	ChangePosition(v.x, v.y);
}

void Player::ChangePositionAndCam(const float _x, const float _y)
{
	//Update the player position
	position.x += _x;
	position.y += _y;
	//Update the camera by the player position
	camera.x += _x;
	camera.y += _y;
}

void Player::ChangePositionAndCam(const Vec2& v)
{
	ChangePositionAndCam(v.x, v.y);
}

void Player::PlayerFixToWorld(const int W, const int H)
{
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
	else if (position.y + playerTexture. GetHeight() > H)
	{
		position.y = float(H - playerTexture.GetHeight());
	}
}

void Player::CameraFixToWorld(const int W, const int H)
{
	if (camera.x < 0)
	{
		camera.x = 0;
	}
	else if (camera.x + Graphics::ScreenWidth > W)
	{
		camera.x = float(W - Graphics::ScreenWidth);
	}
	if (camera.y < 0)
	{
		camera.y = 0;
	}
	else if (camera.y + Graphics::ScreenHeight > H)
	{
		camera.y = float(H - Graphics::ScreenHeight);
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

	PlayerFixToWorld(W, H);
	CameraFixToWorld(W, H);
}

Vec2 Player::GetCamera() const
{
	return camera;
}