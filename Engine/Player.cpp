#include "Player.h"

void Player::ApplyForce(const float _x, const float _y)
{
	ApplyForce(Vec2(_x, _y));
}

void Player::ApplyForce(Vec2 f)
{
	acceleration += f;
}

void Player::Move(const float dt)
{
	//Update the player position
	position += speed * dt;
	//Fix player to world
	const int W = cacheGrid->GetWidth() * Grid::cellWidth;
	const int H = cacheGrid->GetHeight() * Grid::cellHeight;

	if (position.x < 0)
	{
		position.x = 0;
		acceleration *= 0.0f;
		speed *= 0.0f;
	}
	else if (position.x + texture.GetWidth() > W)
	{
		position.x = float(W - texture.GetWidth());
		acceleration *= 0.0f;
		speed *= 0.0f;
	}
	if (position.y < 0)
	{
		position.y = 0;
		acceleration *= 0.0f;
		speed *= 0.0f;
	}
	else if (position.y + texture.GetHeight() > H)
	{
		position.y = float(H - texture.GetHeight());
		acceleration *= 0.0f;
		speed *= 0.0f;
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
		camera.x += speed.x * dt;
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
		camera.y += speed.y * dt;
	}
}

void Player::Physics(const float dt)
{
	//F1 = a1, F2 = a2, F3 = a3;
	//R = F1 + F2 + F3 + ...
	//acceleration = R;
	//speed = acceleration * dt;
	//ChangePositionAndCam(speed * dt);

	//Applying forces
	Vec2 R(acceleration);



	if (cacheGrid->blocks[cacheGrid->GetId(int(position.x / Grid::cellWidth), int(position.y / Grid::cellHeight) + texture.GetHeight() / Grid::cellHeight)].type ==
		Block::Type::Air)
	{
		//ChangePositionAndCam(0.0f, );
	}
}

void Player::MaxAcc()
{
	if (acceleration.GetLengthSq() > maxAcceleration * maxAcceleration)
	{
		acceleration.NormalizeTo(maxAcceleration);
	}
}

void Player::MaxSpeed()
{
	if (speed.GetLengthSq() > maxSpeed * maxSpeed)
	{
		speed.GetNormalizedTo(maxSpeed);
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
	camera(0.0f, 0.0f),
	speed(0.0f, 0.0f),
	acceleration(0.0f, 0.0f)
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

void Player::Update(Keyboard& kbd, Mouse& micky, float dt)
{
	//Destroying and placing blocks
	if (micky.LeftIsPressed())
	{
		//Test to see if the block is in the player range
		const int bX = int((micky.GetPosX() + camera.x) / Grid::cellWidth);
		const int bY = int((micky.GetPosY() + camera.y) / Grid::cellHeight);
		if (abs(bX - position.x / Grid::cellWidth) <= playerRangeX && abs(bY - position.y / Grid::cellHeight) <= playerRangeY)
		{
			cacheGrid->blocks[cacheGrid->GetId(bX, bY)].type = Block::Type::Air;
		}
	}
	else if (micky.RightIsPressed())
	{
		//Test to see if the block is in the player range
		const int bX = int((micky.GetPosX() + camera.x) / Grid::cellWidth);
		const int bY = int((micky.GetPosY() + camera.y) / Grid::cellHeight);
		if (abs(bX - position.x / Grid::cellWidth) <= playerRangeX && abs(bY - position.y / Grid::cellHeight) <= playerRangeY &&
			cacheGrid->blocks[cacheGrid->GetId(bX, bY)].type == Block::Type::Air)
		{
			cacheGrid->blocks[cacheGrid->GetId(bX, bY)].type = Block::Type::Grass;
		}
	}

	//Player movement
	if (kbd.KeyIsPressed('W'))
	{
		ApplyForce(0.0f, -defaultAcc);
	}
	if (kbd.KeyIsPressed('S'))
	{
		ApplyForce(0.0f, defaultAcc);
	}
	if (kbd.KeyIsPressed('A'))
	{
		ApplyForce(-defaultAcc, 0.0f);
	}
	if (kbd.KeyIsPressed('D'))
	{
		ApplyForce(defaultAcc, 0.0f);
	}

	MaxAcc();
	speed += acceleration * dt;
	MaxSpeed();
	Move(dt);
	//Reset acceleration
	acceleration *= 0.0f;
}

Vec2 Player::GetCamera() const
{
	return camera;
}
