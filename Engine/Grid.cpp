#include "Grid.h"
#include <random>
#include "PerlinNoise.h"

Grid::Grid(int width, int height)
	:
	Width(width),
	Height(height)
{
	//Assigning the blocks pointer to an array on the heap
	blocks = new Block[Width * Height];

	//Organizing the blocks
	for (int i = 0; i < Width * Height; i++)
	{
		blocks[i] = Block(Block::Type::Air);
	}

	GenerateSurface();
	FillUnderground();
}

Grid::~Grid()
{
	delete[] blocks;
	blocks = nullptr;
}

void Grid::SaveWorld(char* fileName)
{
	std::ofstream savingStream(fileName, std::ios_base::binary);

	if (savingStream.good())
	{
		//Writing the width and the height of the world
		savingStream.write(reinterpret_cast<char*>(&Width), sizeof(Width));
		savingStream.write(reinterpret_cast<char*>(&Height), sizeof(Height));
		//Writing the blocks data
		for (int i = 0; i < Width * Height; i++)
		{
			savingStream.write(reinterpret_cast<char*>(&blocks[i].type), sizeof(blocks[i].type));
			//Other data will be written here
		}

		//Player location or other things will go after this comment
	}
}

void Grid::LoadWorld(char* fileName)
{
	std::ifstream loadingStream(fileName, std::ios_base::binary);

	if (loadingStream.good())
	{
		//Reading the width and the height of the world
		loadingStream.read(reinterpret_cast<char*>(&Width), sizeof(Width));
		loadingStream.read(reinterpret_cast<char*>(&Height), sizeof(Height));

		//Assigning the blocks pointer to an array on the heap
		blocks = new Block[Width * Height];

		//Reading the blocks data
		for (int i = 0; i < Width * Height; i++)
		{
			loadingStream.read(reinterpret_cast<char*>(&blocks[i].type), sizeof(Block::Type));
		}

		//Player location or other things will go after this comment
	}
}

void Grid::DrawBlocks(Graphics& gfx, int x, int y)
{
	//Find the block that contains that point and then calculate how many blocks I will draw in the horizontal and vertical direction.BOOM EZ!

	const int X = int(x / cellWidth);
	const int Y = int(y / cellHeight);

	for (int j = Y; j < Y + cellsV; j++)
	{
		for (int i = X; i < X + cellsH; i++)
		{
			DrawCell(gfx, i * cellWidth, j * cellHeight, blocks[GetId(i, j)].type);
		}
	}
}

void Grid::GenerateSurface()
{
	std::uniform_real_distribution<float> seedDist(0.0f, 1000000.0f); //You can change the maximum value to whatever you want
	seed = seedDist(std::mt19937(std::random_device{}()));

	for (int i = 0; i < Grid::Width; i++, seed += 0.0800000f) //If you change 0.2f with bigger numbers, it will get suddenly random
	{
		float j = Noise::PerlinNoise_1D(seed, 2.7182818f, 6.2831853f, 1);
		j += float(Grid::Height / 2 - 1);

		blocks[GetId(i, int(j))].type = Block::Type::Grass;
	}
}

void Grid::FillUnderground()
{
}

void Grid::DrawCell(Graphics& gfx, int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
	gfx.DrawRectangle(x, y, x + cellWidth, y + cellHeight, Colors::MakeRGB(r, g, b));
}

void Grid::DrawCell(Graphics& gfx, int x, int y, Block::Type type)
{
	switch (type)
	{
	case Block::Type::Air:
	{
		DrawCell(gfx, x, y, 230, 255, 255);
		break;
	}
	case Block::Type::Dirt:
	{
		DrawCell(gfx, x, y, 191, 128, 64);
		break;
	}
	case Block::Type::Grass:
	{
		DrawCell(gfx, x, y, 0, 102, 34);
		break;
	}
	case Block::Type::Stone:
	{
		DrawCell(gfx, x, y, 163, 163, 117);
		break;
	}
	}
}

int Grid::GetId(int x, int y) const
{
	return y * Width + x;
}

int Grid::GetPosX(int id) const
{
	return id - GetPosY(id);
}

int Grid::GetPosY(int id) const
{
	return int(id / Width);
}
