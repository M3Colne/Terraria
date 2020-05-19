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

float Player::SweptAABB(const float dt) const
{
	
}

bool Player::BroadphasingCollision() const
{
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
	if (velocity.x < -1.0f && velocity.x > 1.0f)
	{
		ApplyForce(frictionForce * -sgn(velocity.x), 0.0f);
	}
	else
	{
		velocity *= 0.0f;
	}

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

	if (BroadphasingCollision())
	{
		//Collision detection
		const float collisionTime = SweptAABB(dt);
		position += velocity * dt * collisionTime;

		//Collision reponse
		if (collisionTime != 1.0f)
		{
			const float remainingTime = 1.0f - collisionTime;

			//Sliding or maybe push, I will see which is better
		}
	}
	else
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
