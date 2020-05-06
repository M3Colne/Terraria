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
	////Find the block that contains that point and then calculate how many blocks I will draw in the horizontal and vertical direction.BOOM EZ!

	//const int X = int(x / cellWidth);
	//const int Y = int(y / cellHeight);

	//////Findig dx,dy
	////const int dx = x - X * cellWidth;
	////const int dy = y - Y * cellHeight;

	//////Draw corners
	//////Top-left
	////DrawCell();
	//////Bottom-left

	//////Top-right
	////
	//////Bottom-right

	//////Draw edges
	////for ();
	////for ();
	////for ();
	////for ();

	//////Draw inside
	////for ();

	for (int j = 0; j < Graphics::ScreenHeight; j += cellHeight)
	{
		for (int i = 0; i < Graphics::ScreenWidth; i += cellWidth)
		{
			DrawCell(gfx, i, j, blocks[GetId(i / cellWidth, j / cellHeight)].type);
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
		//j = 10 * (j * 2 - 1); //j is between 0-1 but after this calculation it will be between -10 and +10
		blocks[GetId(i, int(j * 10))].type = Block::Type::Grass;
	}
}

void Grid::FillUnderground()
{
}

void Grid::DrawCell(Graphics& gfx, int x, int y, Block::Type type)
{
	switch (type)
	{
	case Block::Type::Air:
	{
		gfx.DrawTexture(x, y, Texture2D("./Assets/air.bmp"));
		break;
	}
	case Block::Type::Dirt:
	{
		gfx.DrawTexture(x, y, Texture2D("./Assets/dirt.bmp"));
		break;
	}
	case Block::Type::Grass:
	{
		gfx.DrawTexture(x, y, Texture2D("./Assets/grass.bmp"));
		break;
	}
	case Block::Type::Stone:
	{
		gfx.DrawTexture(x, y, Texture2D("./Assets/stone.bmp"));
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
	return id % Width;
}

int Grid::GetPosY(int id) const
{
	return int(id / Width);
}
