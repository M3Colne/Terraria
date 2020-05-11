#include "Player.h"

Player::Player(const Vei2 pos)
	:
	position(pos),
	playerTexture("./Assets/playerTextureSheet20x40.bmp")
{
}

Player::Player(const int x, const int y)
	:
	position(Vei2(x, y)),
	playerTexture("./Assets/playerTextureSheet20x40.bmp")
{
}

void Player::Draw(Graphics& gfx)
{
	gfx.DrawTextureChroma(position.x, position.y, 0, 0, 20, 40, playerTexture, Colors::Magenta);
}
