#include "Grid.h"
#include "PerlinNoise.h"

Grid::Grid(int width, int height, int surfaceLevel, int surfaceLevelOffset, int maxDirtLayer)
	:
	Width(width),
	Height(height),
	blockSpriteSheet("./Assets/blockSpriteSheet20x20.bmp")
{
	//Assigning the blocks pointer to an array on the heap
	blocks = new Block[Width * Height];

	//Organizing the blocks
	for (int i = 0; i < Width * Height; i++)
	{
		blocks[i] = Block(Block::Type::Air);
	}

	std::mt19937 rng(std::random_device{}());
	GenerateSeed(rng, 0.0f, 1000000.0f);
	GenerateGround(rng, 40, 30, 3);
}

Grid::Grid(char* fileName)
	:
	blockSpriteSheet("./Assets/blockSpriteSheet20x20.bmp")
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

void Grid::DrawBlocks(Graphics& gfx, int x, int y)
{
	const int X = int(x / cellWidth);
	const int Y = int(y / cellHeight);

	for (int j = 0; j <= cellsV; j++)
	{
		for (int i = 0; i <= cellsH; i++)
		{
			DrawCell(gfx, (X + i) * cellWidth - x, (Y + j) * cellHeight - y, blocks[GetId(X + i, Y + j)].type);
		}
	}
}

void Grid::GenerateGround(std::mt19937 rng, int surfaceLevel, int surfaceLevelOffset, int maxDirtLayer)
{
	float s = seed;
	for (int i = 0; i < Grid::Width; i++, s += 0.04000f)
	{
		//Setting the grass
		const int grassOffSet = int(surfaceLevelOffset * Noise::Transform01toN1P1(Noise::PerlinNoise_1D(s, 2.7182818f, 6.2831853f, 1)));
		blocks[GetId(i, grassOffSet + surfaceLevel)].type = Block::Type::Grass;
		std::uniform_int_distribution<int> dirtDistribution(0, maxDirtLayer);
		const int dirtLayers = dirtDistribution(rng);

		//Dirt layer
		for (int d = 0; d < dirtLayers; d++)
		{
			blocks[GetId(i, grassOffSet + surfaceLevel + 1 + d)].type = Block::Type::Dirt;
		}

		//Here I just fill ALL the underground with stone, but later this should be changed with an cave generation algorithm
		//so this is just temporary
		int s = grassOffSet + surfaceLevel + 1 + dirtLayers;
		while (s != Height)
		{
			blocks[GetId(i, s)].type = Block::Type::Stone;
			s++;
		}
	}
}

void Grid::DrawCell(Graphics& gfx, int x, int y, Block::Type type)
{
	switch (type)
	{
	case Block::Type::Air:
	{
		gfx.DrawTexture(x, y, 0, 0, 20, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
		break;
	}
	case Block::Type::Dirt:
	{
		gfx.DrawTexture(x, y, 21, 0, 41, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
		break;
	}
	case Block::Type::Grass:
	{
		gfx.DrawTexture(x, y, 42, 0, 62, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
		break;
	}
	case Block::Type::Stone:
	{
		gfx.DrawTexture(x, y, 63, 0, 83, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
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

void Grid::GenerateSeed(std::mt19937 rng, float min, float max)
{
	std::uniform_real_distribution<float> seedDist(min, max);
	seed = seedDist(rng);
}
