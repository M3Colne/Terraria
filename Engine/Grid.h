#pragma once

#include "Graphics.h"
#include "Block.h"
#include <fstream>

class Grid
{
public:
	//Variables
	static constexpr int cellsH = 100;
	static constexpr int cellsV = 100;
	static constexpr int cellWidth = Graphics::ScreenWidth / cellsH;
	static constexpr int cellHeight = Graphics::ScreenHeight / cellsV;
public:
	//Functions
	Grid() = default;
	Grid(int width, int height);
	Grid(const Grid& other) = delete;
	Grid& operator=(const Grid& other) = delete;
	~Grid();
	void SaveWorld(char* fileName);
	void LoadWorld(char* fileName);
	void DrawBlocks(Graphics& gfx, int x, int y);
	void GenerateSurface();
	void FillUnderground();
	void GenerateOres();
private:
	//Variables
	float seed = 0;
	Block* blocks = nullptr;
	int Width = 0; //In blocks
	int Height = 0; //In blocks
private:
	//Functions
	void DrawCell(Graphics& gfx, int x, int y, Block::Type type);
	int GetId(int x, int y) const;
	int GetPosX(int id) const;
	int GetPosY(int id) const;
};