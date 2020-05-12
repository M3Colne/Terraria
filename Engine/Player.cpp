#include "Player.h"

Player::Player(const float x, const float y)
	:
	position(x, y),
	playerTexture("./Assets/playerTextureSheet20x40.bmp")
{
}

void Player::Draw(Graphics& gfx)
{
	gfx.DrawTextureChroma(dx, dy, 0, 0, playerTexture.GetWidth(), playerTexture.GetHeight(), playerTexture, Colors::Magenta);
}

void Player::Update(bool wP, bool sP, bool aP, bool dP, float dt)
{
	if (wP)
	{
		position.y -= speed * dt;
	}
	if (sP)
	{
		position.y += speed * dt;
	}
	if (aP)
	{
		position.x -= speed * dt;
	}
	if (dP)
	{
		position.x += speed * dt;
	}
}

Vec2 Player::GetCamera() const
{
	return position - Vec2(float(dx), float(dy)); //This returns cworld
}