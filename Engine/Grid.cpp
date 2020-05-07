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
	const int X = int(x / cellWidth);
	const int Y = int(y / cellHeight);

	for (int j = 0; j < cellsV; j++)
	{
		for (int i = 0; i < cellsH; i++)
		{
			DrawCell(gfx, (X + i) * cellWidth - x, (Y + j) * cellHeight - y, blocks[GetId(X + i, Y + j)].type);
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
	Texture2D blockTexture("./Assets/air.bmp");
	switch (type)
	{
	case Block::Type::Dirt:
	{
		blockTexture = Texture2D("./Assets/dirt.bmp");
		break;
	}
	case Block::Type::Grass:
	{
		blockTexture = Texture2D("./Assets/grass.bmp");
		break;
	}
	case Block::Type::Stone:
	{
		blockTexture = Texture2D("./Assets/stone.bmp");
		break;
	}
	}

	gfx.DrawTexture(x, y, 0, 0, blockTexture.GetWidth(), blockTexture.GetHeight(),
					0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockTexture);
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
