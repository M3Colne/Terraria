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
	Grid(int width, int height, int surfaceLevel, int surfaceLevelOffset, int maxDirtLayer);
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
	void GenerateGround(int surfaceLevel, int surfaceLevelOffset, int minDirtLayer, int maxDirtLayer);
	void GenerateOres(float chanceOfSpawn, float chanceOfCluster, float chanceDivisor, int minLevel, int maxLevel, Block::Type bType);
};