#include "Player.h"

void Player::ChangePositionAndCam(const float _x, const float _y)
{
	//Update the player position
	position.x += _x;
	position.y += _y;
	//Fix player to world
	const int W = cacheGrid->GetWidth() * Grid::cellWidth;
	const int H = cacheGrid->GetHeight() * Grid::cellHeight;

	if (position.x < 0)
	{
		position.x = 0;
	}
	else if (position.x + texture.GetWidth() > W)
	{
		position.x = float(W - texture.GetWidth());
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	else if (position.y + texture.GetHeight() > H)
	{
		position.y = float(H - texture.GetHeight());
	}

	//Fix camera to world
	if (position.x <= dx)
	{
		camera.x = 0.0f;
	}
	else if (position.x >= W - texture.GetWidth() - dx)
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
	else if (position.y >= H - texture.GetHeight() - dy)
	{
		camera.y = float(H - Graphics::ScreenHeight);
	}
	else
	{
		camera.y += _y;
	}
}

Vec2 Player::GetPosition() const
{
	return position;
}

Player::Player(Grid& grid, const int x)
	:
	texture("./Assets/playerTextureSheet20x40.bmp"),
	cacheGrid(&grid),
	position(0.0f, 0.0f),
	camera(0.0f, 0.0f)
{
	//Finding the y position
	int BLOCKY = 0;
	while (true)
	{
		if (cacheGrid->blocks[x + BLOCKY * cacheGrid->GetWidth()].type == Block::Type::Grass)
		{
			break;
		}
		BLOCKY++;
	}

	//Setting position
	position = Vec2(float(x * Grid::cellWidth), float((BLOCKY - 2) * Grid::cellHeight)); // - 2 because the player is 2 blocks tall
	//Setting camera
	camera = position - Vec2(float(dx), float(dy));
}

Player::~Player()
{
	delete cacheGrid;
	cacheGrid = nullptr;
}

void Player::Draw(Graphics& gfx)
{
	gfx.DrawTextureChroma(int(position.x - camera.x), int(position.y - camera.y),
		0, 0, texture.GetWidth(), texture.GetHeight(), texture, Colors::Magenta);
}

Vec2 Player::GetCamera() const
{
	return camera;
}
