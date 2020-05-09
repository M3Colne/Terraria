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

	GenerateSeed(0, 1000000);
	//Generate the world based on this seed
	//std::mt19937 rng(seed);
	//GenerateGround(rng, 40, 15, 3, 6);
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

void Grid::GenerateGround(std::mt19937& r, int surfaceLevel, int surfaceLevelOffset, int minDirtLayer, int maxDirtLayer)
{
	float tempS = float(seed);
	std::uniform_int_distribution<int> dirtLayerDist(minDirtLayer, maxDirtLayer);
	for (int i = 0; i < Grid::Width; i++, tempS += 0.0180f)
	{
		//Setting the grass
		const int grassOffSet = int(Noise::PerlinNoise_1D(tempS, 2.7182818f, surfaceLevelOffset, 1, true));
		blocks[GetId(i, grassOffSet + surfaceLevel)].type = Block::Type::Grass;
		const int dirtLayers = dirtLayerDist(r);

		//Dirt layer
		for (int d = 0; d < dirtLayers; d++)
		{
			blocks[GetId(i, grassOffSet + surfaceLevel + 1 + d)].type = Block::Type::Dirt;
		}

		//Here I just fill ALL the underground with stone, but later this should be changed with an cave generation algorithm
		//so this is just temporary
		int st = grassOffSet + surfaceLevel + 1 + dirtLayers;
		while (st != Height)
		{
			blocks[GetId(i, st)].type = Block::Type::Stone;
			st++;
		}
	}

	GenerateOres(r, 0.0015f, 80.0f, 2.0f, 0, surfaceLevel + surfaceLevelOffset + maxDirtLayer + 45, Block::Type::Grass);
}

void Grid::GenerateOres(std::mt19937& r, float chanceOfSpawn, float chanceOfCluster, float chanceDivisor, int minLevel, int maxLevel, Block::Type bType)
{
	std::vector<int> oresId;
	std::uniform_real_distribution<float> chanceDist(0.0f, 100.0f);
	for (int j = minLevel; j < maxLevel; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			const int blockId = GetId(i, j);
			if (blocks[blockId].type == Block::Type::Stone && chanceDist(r) < chanceOfSpawn)
			{
				oresId.push_back(blockId);
			}
		}
	}

	std::vector<int> branchId(oresId.size());
	for (int source = 0; source < oresId.size(); source++)
	{
		for (int branch = 0; branch < branchId.size(); branch++)
		{
			const int s = branchId[branch];
			const int t = s - Width;
			const int b = s + Width;
			const int l = s - 1;
			const int r = s + 1;

			if (t >= 0 && blocks[t].type == Block::Type::Stone && chanceDist(r) <= chanceOfCluster)
			{
				blocks[t].type = bType;
				branchId.push_back(t);
			}
			else if (b < Height && blocks[b].type == Block::Type::Stone && chanceDist(r) <= chanceOfCluster)
			{
				blocks[b].type = bType;
				branchId.push_back(b);
			}
			else if (l >= 0 && blocks[l].type == Block::Type::Stone && chanceDist(r) <= chanceOfCluster)
			{
				blocks[l].type = bType;
				branchId.push_back(l);
			}
			else if (r < Width && blocks[r].type == Block::Type::Stone && chanceDist(r) <= chanceOfCluster)
			{
				blocks[t].type = bType;
				branchId.push_back(l);
			}

			chanceOfCluster /= chanceDivisor;
		}
		branchId.clear();
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

void Grid::GenerateSeed(int min, int max)
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> seedDist(min, max);
	seed = seedDist(rng);
}
