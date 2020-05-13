#include "Grid.h"
#include "PerlinNoise.h"
#include <random>

Grid::Grid(int width, int height, const int surfaceLevel, const int surfaceLevelOffset, const int minDirtLayer, const int maxDirtLayer)
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

	GenerateSeed(0, 100000);
	//Generate the world based on this seed
	GenerateGround(surfaceLevel, surfaceLevelOffset, minDirtLayer, maxDirtLayer);
	GenerateOres(0.05f, 300.0f, 1.14f, surfaceLevel+surfaceLevelOffset + maxDirtLayer,
		surfaceLevel + surfaceLevelOffset + maxDirtLayer + 150, Block::Type::Copper);
}

Grid::Grid(char* fileName)
	:
	blockSpriteSheet("./Assets/blockSpriteSheet20x20.bmp")
{
	std::ifstream loadingStream(fileName, std::ios_base::binary);

	if (loadingStream.good())
	{
		//Reading the width and the height of the world
		int ncomp = 0;
		loadingStream.read(reinterpret_cast<char*>(&ncomp), sizeof(ncomp));
		loadingStream.read(reinterpret_cast<char*>(&seed), sizeof(seed));
		loadingStream.read(reinterpret_cast<char*>(&Width), sizeof(Width));
		loadingStream.read(reinterpret_cast<char*>(&Height), sizeof(Height));

		//Assigning the blocks pointer to an array on the heap
		blocks = new Block[Width * Height];

		//Reading the compressed blocks data
		int lastI = 0;
		for (int c = 0; c < ncomp; c++)
		{
			int n = 1;
			Block::Type t;
			loadingStream.read(reinterpret_cast<char*>(&n), sizeof(n));
			loadingStream.read(reinterpret_cast<char*>(&t), sizeof(t));
			for (int i = lastI; i < lastI + n; i++)
			{
				blocks[i].type = t; //If the world file is good(no missing data or anything esle) then this warning should be ignored
									//it's just reminding that it's dangerous to read data like this
			}
			lastI += n;
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
	//Row compression method
	std::ofstream savingStream(fileName, std::ios_base::binary);

	if (savingStream.good())
	{
		//World and compressed block data
		{
			//This is a variable that is used when loading. I am writing it at the start with 0 but Im going to come back and change it
			//with the real number because I can't know in advance how many block data compressions will be
			//So I just keep this 4 bytes with the number 0 but I will return here and change it with the real number
			int ncomp = 0;

			//Writing the width and the height of the world
			savingStream.write(reinterpret_cast<char*>(&ncomp), sizeof(ncomp));
			savingStream.write(reinterpret_cast<char*>(&seed), sizeof(seed));
			savingStream.write(reinterpret_cast<char*>(&Width), sizeof(Width));
			savingStream.write(reinterpret_cast<char*>(&Height), sizeof(Height));
			//Writing compressed block data
			int n = 0;
			const int size = Width * Height - 1;
			for (int i = 0; i < size; i++)
			{
				if (blocks[i + 1].type == blocks[i].type)
				{
					n++;
				}
				else
				{
					savingStream.write(reinterpret_cast<char*>(&n), sizeof(n));
					savingStream.write(reinterpret_cast<char*>(&blocks[i].type), sizeof(blocks[i].type));
					n = 1;
					ncomp++;
				}
			}
			n = 1;
			//Write the last block manually because the algorithm doesn't work with the last i
			savingStream.write(reinterpret_cast<char*>(&n), sizeof(n));
			savingStream.write(reinterpret_cast<char*>(&blocks[size].type), sizeof(blocks[size].type));
			ncomp++;

			//Write how many compressions have been made
			{
				const auto k = savingStream.tellp();
				savingStream.seekp(0);
				savingStream.write(reinterpret_cast<char*>(&ncomp), sizeof(ncomp)); //Overwrite the 0 with the actual number
				savingStream.seekp(k);
			}
		}

		//Any other data like the player or time or space or anything goes below this comment
		//If it's a repetitive data like the block types then you should the same algorithm
	}
}

void Grid::DrawBlocks(Graphics& gfx, int x, int y)
{
	const int X = int(x / cellWidth);
	const int Y = int(y / cellHeight);
	assert(X >= 0);
	assert(X < Width);
	assert(Y >= 0);
	assert(Y < Height);

	for (int j = 0; j <= cellsV; j++)
	{
		for (int i = 0; i <= cellsH; i++)
		{
			DrawCell(gfx, (X + i) * cellWidth - x, (Y + j) * cellHeight - y, blocks[GetId(X + i, Y + j)].type);
		}
	}
}

void Grid::DrawBlocks(Graphics& gfx, const Vec2& gPos)
{
	DrawBlocks(gfx, int(gPos.x), int(gPos.y));
}

int Grid::GetHeight() const
{
	return Height;
}

int Grid::GetWidth() const
{
	return Width;
}

void Grid::GenerateGround(const int surfaceLevel, const int surfaceLevelOffset, const int minDirtLayer, const int maxDirtLayer)
{
	std::mt19937 rng(seed);
	float tempS = float(seed);
	std::uniform_int_distribution<int> dirtLayerDist(minDirtLayer, maxDirtLayer);
	for (int i = 0; i < Grid::Width; i++, tempS += 0.0180f)
	{
		//Setting the grass
		const int grassOffSet = int(Noise::PerlinNoise_1D(tempS, 2.7182818f, float(surfaceLevelOffset), 1, true));
		blocks[GetId(i, grassOffSet + surfaceLevel)].type = Block::Type::Grass;

		//Dirt layer
		const int dirtLayers = dirtLayerDist(rng);
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
}

void Grid::GenerateOres(const float chanceOfSpawn, const float chanceOfCluster, const float chanceDivisor, const int minLevel,
	const int maxLevel, const Block::Type bType)
{
	std::mt19937 rng(seed);
	std::vector<int> oresId;
	std::uniform_real_distribution<float> chanceDist(0.0f, 100.0f);
	for (int j = minLevel; j < maxLevel; j++)
	{
		for (int i = 0; i < Width; i++)
		{
			const int blockId = GetId(i, j);
			if (blocks[blockId].type == Block::Type::Stone && chanceDist(rng) < chanceOfSpawn)
			{
				oresId.push_back(blockId);
			}
		}
	}

	std::vector<int> branchId;
	for (unsigned int source = 0; source < oresId.size(); source++)
	{
		branchId.push_back(oresId[source]);
		float c = chanceOfCluster;

		for (unsigned int branch = 0; branch < branchId.size(); branch++)
		{
			const int so = branchId[branch];
			const int top = so - Width;
			const int bot = so + Width;
			const int left = so - 1;
			const int right = so + 1;

			if (top >= 0 && blocks[top].type == Block::Type::Stone && chanceDist(rng) <= c)
			{
				blocks[top].type = bType;
				branchId.push_back(top);
			}
			if (GetPosY(bot) < Height && blocks[bot].type == Block::Type::Stone && chanceDist(rng) <= c)
			{
				blocks[bot].type = bType;
				branchId.push_back(bot);
			}
			if (left >= 0 && blocks[left].type == Block::Type::Stone && chanceDist(rng) <= c)
			{
				blocks[left].type = bType;
				branchId.push_back(left);
			}
			if (GetPosX(right) < Width && blocks[right].type == Block::Type::Stone && chanceDist(rng) <= c)
			{
				blocks[right].type = bType;
				branchId.push_back(right);
			}

			c /= chanceDivisor;
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
		gfx.DrawTexture(x, y, 20, 0, 40, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
		break;
	}
	case Block::Type::Grass:
	{
		gfx.DrawTexture(x, y, 40, 0, 60, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
		break;
	}
	case Block::Type::Stone:
	{
		gfx.DrawTexture(x, y, 60, 0, 80, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
		break;
	}
	case Block::Type::Copper:
	{
		gfx.DrawTexture(x, y, 80, 0, 100, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet);
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
