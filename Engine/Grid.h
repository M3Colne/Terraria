#pragma once

#include "Graphics.h"
#include "Block.h"
#include <fstream>
#include <random>

class Grid
{
public:
	//Variables
	static constexpr int cellsH = 40;
	static constexpr int cellsV = 30;
	static constexpr int cellWidth = Graphics::ScreenWidth / cellsH;
	static constexpr int cellHeight = Graphics::ScreenHeight / cellsV;
public:
	//Functions
	Grid(int width, int height, const int surfaceLevel, const int surfaceLevelOffset, const int minDirtLayer, const int maxDirtLayer);
	Grid(char* fileName);
	Grid(const Grid& other) = delete;
	Grid& operator=(const Grid& other) = delete;
	~Grid();
	void SaveWorld(char* fileName);
	void DrawBlocks(Graphics& gfx, int x, int y);
private:
	//Variables
	Texture2D blockSpriteSheet;
	int seed = 0;
	Block* blocks = nullptr;
	int Width = 0; //In blocks
	int Height = 0; //In blocks
private:
	//Functions
	void DrawCell(Graphics& gfx, int x, int y, Block::Type type);
	int GetId(int x, int y) const;
	int GetPosX(int id) const;
	int GetPosY(int id) const;
	void GenerateSeed(int min, int max);
	void GenerateGround(const int surfaceLevel, const int surfaceLevelOffset,const int minDirtLayer, const int maxDirtLayer);
	void GenerateOres(const float chanceOfSpawn, const float chanceOfCluster, const float chanceDivisor, const int minLevel,
		const int maxLevel, const Block::Type bType);
};