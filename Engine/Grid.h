#pragma once

#include "Graphics.h"
#include "Block.h"
#include <fstream>

class Grid
{
public:
	//Variables
	static constexpr int cellsH = 40;
	static constexpr int cellsV = 30;
	static constexpr int cellWidth = Graphics::ScreenWidth / cellsH;
	static constexpr int cellHeight = Graphics::ScreenHeight / cellsV;
	Block* blocks = nullptr;
public:
	//Functions
	Grid() = default;
	Grid(int width, int height, const int surfaceLevel, const int surfaceLevelOffset, const int minDirtLayer, const int maxDirtLayer);
	Grid(const char* fileName);
	Grid(const Grid& other) = delete;
	Grid& operator=(const Grid& other) = delete;
	~Grid();
	void SaveWorld(char* fileName);
	void DrawBlocks(Graphics& gfx, int x, int y);
	int GetHeight() const;
	int GetWidth() const;
	int GetId(int x, int y) const;
	int GetPosX(int id) const;
	int GetPosY(int id) const;
	template <typename E> void DrawCell(Graphics& gfx, int x, int y, Block::Type type, E e)
	{
		switch (type)
		{
		case Block::Type::Air:
		{
			gfx.DrawTexture(x, y, 0, 0, 20, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet, e);
			break;
		}
		case Block::Type::Dirt:
		{
			gfx.DrawTexture(x, y, 20, 0, 40, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet, e);
			break;
		}
		case Block::Type::Grass:
		{
			gfx.DrawTexture(x, y, 40, 0, 60, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet, e);
			break;
		}
		case Block::Type::Stone:
		{
			gfx.DrawTexture(x, y, 60, 0, 80, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet, e);
			break;
		}
		case Block::Type::Copper:
		{
			gfx.DrawTexture(x, y, 80, 0, 100, 20, 0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, blockSpriteSheet, e);
			break;
		}
		}
	}
private:
	//Variables
	const Texture2D blockSpriteSheet;
	int seed = 0;
	int Width = 0; //In blocks
	int Height = 0; //In blocks
private:
	//Functions
	void GenerateSeed(int min, int max);
	void GenerateGround(const int surfaceLevel, const int surfaceLevelOffset,const int minDirtLayer, const int maxDirtLayer);
	void GenerateOres(const float chanceOfSpawn, const float chanceOfCluster, const float chanceDivisor, const int minLevel,
		const int maxLevel, const Block::Type bType);
};