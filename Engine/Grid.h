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
	Grid(char* fileName);
	Grid(const Grid& other) = delete;
	Grid& operator=(const Grid& other) = delete;
	~Grid();
	void SaveWorld(char* fileName);
	void DrawBlocks(Graphics& gfx, int x, int y);
	void DrawBlocks(Graphics& gfx, const Vec2& gPos);
	int GetHeight() const;
	int GetWidth() const;
	static int GetId(int x, int y);
	static int GetPosX(int id);
	static int GetPosY(int id);
private:
	//Variables
	const Texture2D blockSpriteSheet;
	int seed = 0;
	static int Width = 0; //In blocks
	static int Height = 0; //In blocks
private:
	//Functions
	void DrawCell(Graphics& gfx, int x, int y, Block::Type type);
	void GenerateSeed(int min, int max);
	void GenerateGround(const int surfaceLevel, const int surfaceLevelOffset,const int minDirtLayer, const int maxDirtLayer);
	void GenerateOres(const float chanceOfSpawn, const float chanceOfCluster, const float chanceDivisor, const int minLevel,
		const int maxLevel, const Block::Type bType);
};