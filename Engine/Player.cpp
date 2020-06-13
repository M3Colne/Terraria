#include "Player.h"
#include "SpriteEffects.h"

int Player::sgn(float v) const
{
	return (v > 0.0f) ? 1 : ((v < 0.0f) ? -1 : 0);
}

void Player::ApplyForce(const float _x, const float _y)
{
	ApplyForce(Vec2(_x, _y));
}

void Player::ApplyForce(Vec2 f)
{
	acceleration += f;
}

void Player::StopX()
{
	acceleration.x = 0.0f;
	velocity.x = 0.0f;
}

void Player::StopY()
{
	acceleration.y = 0.0f;
	velocity.y = 0.0f;
}

void Player::Collisions(bool& COLL, const float dt)
{
	//Calculating the broadphase zone
	int left = int((position.x + (velocity.x >= 0.0f ? 0.0f : velocity.x)) / Grid::cellWidth);
	int right = int((position.x + texture.GetWidth() + (velocity.x > 0.0f ? velocity.x + Grid::cellWidth : 0.0f)) / Grid::cellWidth);
	int top = int((position.y + (velocity.y >= 0.0f ? 0.0f : velocity.y)) / Grid::cellHeight);
	int bottom = int((position.y + texture.GetHeight() + (velocity.y > 0.0f ? velocity.y + Grid::cellHeight : 0.0f)) / Grid::cellHeight);

	if (left < 0)
	{
		left = 0;
	}
	if (right >= cacheGrid->GetWidth())
	{
		right = cacheGrid->GetWidth() - 1;
	}
	if (top < 0)
	{
		top = 0;
	}
	if (bottom >= cacheGrid->GetHeight())
	{
		bottom = cacheGrid->GetHeight() - 1;
	}

	float collisionTime = 1.0f;
	Vec2 normal(0.0f, 0.0f);

	for (int j = top; j < bottom; j++)
	{
		for (int i = left; i < right; i++)
		{
			const int id = cacheGrid->GetId(i, j);
			if (cacheGrid->blocks[id].type != Block::Type::Air)
			{
				const float t = SweptAABB(id, normal, dt);
				
				if (t < collisionTime)
				{
					collisionTime = t;
				}
			}
		}
	}

	if (collisionTime != 1.0f)
	{
		//I use this variable just in case the player didn't collided with anything
		COLL = true;
		
		if (normal.y == -1.0f)
		{
			onGround = true;
		}

		//Collision response
		position += velocity * dt * collisionTime;

		// slide
		const float remainingTime = 1.0f - collisionTime;
		const float dotprod = (velocity.x * normal.y + velocity.y * normal.x) * remainingTime;
		velocity.x = dotprod * normal.y;
		velocity.y = dotprod * normal.x;

		position += velocity * dt * remainingTime;

		//Stop the players forces when he collides with a wall
		if (normal.x != 0.0f)
		{
			StopX();
		}
		else if (normal.y != 0.0f)
		{
			StopY();
		}
	}
}

float Player::SweptAABB(const int id, Vec2& n, const float dt) const
{
	const Vec2 tempV(velocity * dt);
	float entryDistX, entryDistY, exitDistX, exitDistY;
	{
		const int left = cacheGrid->GetPosX(id) * Grid::cellWidth;
		const int right = left + Grid::cellWidth;
		const int top = cacheGrid->GetPosY(id) * Grid::cellHeight;
		const int bottom = top + Grid::cellHeight;
		//Get distance
		if (tempV.x > 0.0f)
		{
			entryDistX = left - (position.x + texture.GetWidth());
			exitDistX = right - position.x;
		}
		else
		{
			entryDistX = right - position.x;
			exitDistX = left - (position.x + texture.GetWidth());
		}
		if (tempV.y > 0.0f)
		{
			entryDistY = top - (position.y + texture.GetHeight());
			exitDistY = bottom - position.y;
		}
		else
		{
			entryDistY = bottom - position.y;
			exitDistY = top - (position.y + texture.GetHeight());
		}
	}

	//Get time on each axis
	float entryTimeX, entryTimeY, exitTimeX, exitTimeY;
	if (tempV.x == 0.0f)
	{
		entryTimeX = -std::numeric_limits<float>::infinity();
		exitTimeX = std::numeric_limits<float>::infinity();
	}
	else
	{
		entryTimeX = entryDistX / tempV.x;
		exitTimeX = exitDistX / tempV.x;
	}
	if (tempV.y == 0.0f)
	{
		entryTimeY = -std::numeric_limits<float>::infinity();
		exitTimeY = std::numeric_limits<float>::infinity();
	}
	else
	{
		entryTimeY = entryDistY / tempV.y;
		exitTimeY = exitDistY / tempV.y;
	}

	//Get time first and last time
	const float entryTime = std::max<float>(entryTimeX, entryTimeY);
	const float exitTime = std::min<float>(exitTimeX, exitTimeY);

	//Collision detection
	if (entryTime > exitTime || entryTimeX < 0.0f && entryTimeY < 0.0f || entryTimeX > 1.0f || entryTimeY > 1.0f)
	{
		return 1.0f;
	}

	if (entryTimeX > entryTimeY)
	{
		if (entryDistX < 0.0f)
		{
			n = { 1.0f, 0.0f };
		}
		else
		{
			n = { -1.0f, 0.0f };
		}
	}
	else
	{
		if (entryDistY < 0.0f)
		{
			n = { 0.0f, 1.0f };
		}
		else
		{
			n = { 0.0f, -1.0f };
		}
	}

	return entryTime;
}

void Player::SavePlayer(const char* fileName) const
{
	std::ofstream out(fileName, std::ios::binary);

	if (out.good())
	{
		out.write((PCHAR)&position, sizeof(position));
		out.write((PCHAR)&velocity, sizeof(velocity));
		out.write((PCHAR)&acceleration, sizeof(acceleration));
		out.write((PCHAR)&framesInAir, sizeof(framesInAir));
		if (onGround)
		{
			out.put(1);
		}
		else
		{
			out.put(0);
		}
	}
	else
	{
		throw 2;
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
	velocity(0.0f, 0.0f),
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
	position = Vec2(float(x * Grid::cellWidth), float(BLOCKY * Grid::cellHeight - texture.GetHeight()));
}

Player::Player(const char* fileName, Grid& grid)
	:
	texture("./Assets/playerTextureSheet20x40.bmp"),
	cacheGrid(&grid),
	position(0.0f, 0.0f),
	velocity(0.0f, 0.0f),
	acceleration(0.0f, 0.0f)
{
	std::ifstream in(fileName, std::ios::binary);
	if (in.good())
	{
		in.read((PCHAR)&position, sizeof(position));
		in.read((PCHAR)&velocity, sizeof(velocity));
		in.read((PCHAR)&acceleration, sizeof(acceleration));
		in.read((PCHAR)&framesInAir, sizeof(framesInAir));
		if (in.get() == 1)
		{
			onGround = true;
		}
		else
		{
			onGround = false;
		}
	}
	else
	{
		throw 3;
	}
}

Player::~Player()
{
	//There is no need to delete the cacheGrid data here because its deleted in from game.cpp
}

void Player::Draw(Graphics& gfx)
{
	gfx.DrawTexture(int(position.x - GetCameraX()), int(position.y - GetCameraY()),
		0, 0, texture.GetWidth(), texture.GetHeight(), texture, SpriteEffects::Chroma(Colors::Magenta));
}

void Player::Update(Keyboard& kbd, Mouse& micky, const float dt)
{
	//Destroying and placing blocks
	if (micky.LeftIsPressed())
	{
		//Test to see if the block is in the player range
		const int bX = int((micky.GetPosX() + GetCameraX()) / Grid::cellWidth);
		const int bY = int((micky.GetPosY() + GetCameraY()) / Grid::cellHeight);
		if (abs(bX - position.x / Grid::cellWidth) <= playerRangeX && abs(bY - position.y / Grid::cellHeight) <= playerRangeY)
		{
			cacheGrid->blocks[cacheGrid->GetId(bX, bY)].type = Block::Type::Air;
		}
	}
	else if (micky.RightIsPressed())
	{
		//Test to see if the block is in the player range
		const int bX = int((micky.GetPosX() + GetCameraX()) / Grid::cellWidth);
		const int bY = int((micky.GetPosY() + GetCameraY()) / Grid::cellHeight);
		if (abs(bX - position.x / Grid::cellWidth) <= playerRangeX && abs(bY - position.y / Grid::cellHeight) <= playerRangeY &&
			cacheGrid->blocks[cacheGrid->GetId(bX, bY)].type == Block::Type::Air)
		{
			cacheGrid->blocks[cacheGrid->GetId(bX, bY)].type = Block::Type::Grass;
		}
	}

	//Horizontal and vertica forces
	if (kbd.KeyIsPressed('W'))
	{
		onGround = false;
		position.y -= (initialJumpImpulse - framesInAir) * cacheGrid->cellHeight * dt;
	}
	if (kbd.KeyIsPressed('D'))
	{
		if (velocity.x < 0.0f)
		{
			ApplyForce(defaultDeacc, 0.0f);
			if (velocity.x + acceleration.x * dt > 0.0f)
			{
				velocity.x = 0.0f;
			}
		}
		else
		{
			ApplyForce(defaultAcc, 0.0f);
		}
	}
	else
	{
		const int oldS = sgn(velocity.x);
		velocity.x -= oldS * frictionForce * dt;
		if (sgn(velocity.x) != oldS)
		{
			StopX();
		}
	}
	if (kbd.KeyIsPressed('A'))
	{
		if (velocity.x > 0.0f)
		{
			ApplyForce(-defaultDeacc, 0.0f);
			if (velocity.x - defaultDeacc * dt < 0.0f)
			{
				velocity.x = 0.0f;
			}
		}
		else
		{
			ApplyForce(-defaultAcc, 0.0f);
		}
	}
	else
	{
		const int oldS = sgn(velocity.x);
		velocity.x -= oldS * frictionForce * dt;
		if (sgn(velocity.x) != oldS)
		{
			StopX();
		}
	}
	//Gravity
	if (cacheGrid->blocks[cacheGrid->GetId(int(position.x / Grid::cellWidth), int(position.y / Grid::cellHeight) + texture.GetHeight() / Grid::cellHeight)].type ==
		Block::Type::Air)
	{
		ApplyForce(0.0f, gravity);
	}

	//Max acceleration
	if (acceleration.GetLengthSq() > maxAcceleration * maxAcceleration)
	{
		acceleration.NormalizeTo(maxAcceleration);
	}
	velocity += acceleration * dt;
	//Max speed
	if (abs(velocity.x) > maxSpeedX)
	{
		velocity.x = (velocity.x / abs(velocity.x)) * maxSpeedX;
	}
	if (abs(velocity.y) > maxSpeedY)
	{
		velocity.y = (velocity.y / abs(velocity.y)) * maxSpeedY;
	}

	//Check for collision in the broadphase area
	bool collided = false;
	Collisions(collided, dt);

	//I use this variable just so I don't update 2 times in the same frame
	if (!collided)
	{
		position += velocity * dt;
	}

	//Fix the player to the world
	const int W = cacheGrid->GetWidth() * Grid::cellWidth;
	const int H = cacheGrid->GetHeight() * Grid::cellHeight;
	if (position.x < 0)
	{
		position.x = 0.0f;
		StopX();
	}
	else if (position.x + texture.GetWidth() > W)
	{
		position.x = float(W - texture.GetWidth());
		StopX();
	}
	if (position.y < 0)
	{
		position.y = 0;
		StopY();
	}
	else if (position.y + texture.GetHeight() > H)
	{
		position.y = float(H - texture.GetHeight());
		StopY();
	}

	//Reset acceleration
	acceleration *= 0.0f;
}

int Player::GetCameraX() const
{
	const int x = int(position.x) - Graphics::ScreenWidth / 2 - texture.GetWidth() / 2;
	const int W = cacheGrid->cellWidth * cacheGrid->GetWidth() - Graphics::ScreenWidth;
	if (x < 0.0f)
	{
		return 0;
	}
	else if (x >= W)
	{
		return W;
	}
	else
	{
		return int(position.x) - Graphics::ScreenWidth / 2 - texture.GetWidth() / 2;
	}
}

int Player::GetCameraY() const
{
	const int y = int(position.y) - Graphics::ScreenHeight/ 2 - texture.GetHeight() / 2;
	const int H = cacheGrid->cellHeight* cacheGrid->GetHeight() - Graphics::ScreenHeight;
	if (y < 0.0f)
	{
		return 0;
	}
	else if (y >= H)
	{
		return H;
	}
	else
	{
		return int(position.y) - Graphics::ScreenHeight / 2 - texture.GetHeight() / 2;
	}
}
