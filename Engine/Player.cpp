#include "Player.h"

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

void Player::Fix(const float dt)
{
	//Fix player to world
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
		camera.x += velocity.x * dt;
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
		camera.y += velocity.y * dt;
	}
}

void Player::Collisions(bool& COLL, const float dt)
{
	//Check for collision in the broadphase area (the broadphasing zone is the entire screen, sadly)
	//I want the minimum collisionTime from every block(that's the closest block that the player collied)
	float collisionTime = 1.0f;
	Vec2 normal(0.0f, 0.0f);
	for (int j = 0; j < Grid::cellsV; j++)
	{
		for (int i = 0; i < Grid::cellsH; i++)
		{
			const int id = cacheGrid->GetId(i + int(camera.x / Grid::cellWidth), j + int(camera.y / Grid::cellHeight));
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

		//Collision response
		position += velocity * dt * collisionTime;

		//Stop the players forces when he collides with a wall
		/*if (normal.x != 0.0f)
		{
			StopX();
		}
		else if (normal.y != 0.0f)
		{
			StopY();
		}*/
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

void Player::Update(Keyboard& kbd, Mouse& micky, const float dt)
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

	//Forces
	if (kbd.KeyIsPressed('W'))
	{
		ApplyForce(0.0f, -defaultAcc);
	}
	if (kbd.KeyIsPressed('A'))
	{
		if (velocity.x > 0.0f)
		{
			ApplyForce(-defaultDeacc, 0.0f);
		}
		else
		{
			ApplyForce(-defaultAcc, 0.0f);
		}
	}
	if (kbd.KeyIsPressed('D'))
	{
		if (velocity.x < 0.0f)
		{
			ApplyForce(defaultDeacc, 0.0f);
		}
		else
		{
			ApplyForce(defaultAcc, 0.0f);
		}
	}
	//Gravity
	if (cacheGrid->blocks[cacheGrid->GetId(int(position.x / Grid::cellWidth), int(position.y / Grid::cellHeight) + texture.GetHeight() / Grid::cellHeight)].type ==
		Block::Type::Air)
	{
		ApplyForce(0.0f, gravity);
	}
	//Friction
	/*if (velocity.x < -1.0f && velocity.x > 1.0f)
	{
		ApplyForce(frictionForce * -sgn(velocity.x), 0.0f);
	}
	else
	{
		velocity *= 0.0f;
	}*/

	//Max acceleration
	if (acceleration.GetLengthSq() > maxAcceleration * maxAcceleration)
	{
		acceleration.NormalizeTo(maxAcceleration);
	}
	velocity += acceleration * dt;
	//Max speed
	if (velocity.GetLengthSq() > maxSpeed * maxSpeed)
	{
		velocity.GetNormalizedTo(maxSpeed);
	}

	//Check for collision in the broadphase area
	bool collided = false;
	Collisions(collided, dt);
	
	//I use this variable just so I don't update 2 times in the same frame
	if (!collided)
	{
		position += velocity * dt;
	}

	Fix(dt);

	//Reset acceleration
	acceleration *= 0.0f;
}

Vec2 Player::GetCamera() const
{
	return camera;
}
