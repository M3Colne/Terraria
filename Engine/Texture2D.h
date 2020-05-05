#pragma once

#include "Colors.h"
#include <assert.h>
#include <fstream>

struct Texture2D
{
private:
	//Variables
	Color* pixels = nullptr;
	int width = 0;
	int height = 0;
private:
	//Functions
public:
	//Variables
public:
	//Functions
	Texture2D(const char* fileName)
	{
		std::ifstream bmpStream(fileName, std::ios::binary);

		BITMAPFILEHEADER bmpFileHeader;

		bmpStream.read(reinterpret_cast<char*>(&bmpFileHeader), sizeof(bmpFileHeader));

		BITMAPINFOHEADER bmpInfoHeader;

		bmpStream.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

		assert(bmpInfoHeader.biBitCount == 24);
		assert(bmpInfoHeader.biCompression == BI_RGB);

		this->width = bmpInfoHeader.biWidth;
		this->height = bmpInfoHeader.biHeight;
		this->pixels = new Color[width * height];

		bmpStream.seekg(bmpFileHeader.bfOffBits, std::ios::beg);
		const int padding = (4 - (width * 3) % 4) % 4;

		for (int j = height - 1; j >= 0; j--)
		{
			for (int i = 0; i < width; i++)
			{
				SetPixel(i, j, Color( bmpStream.get(), bmpStream.get(),bmpStream.get() ));
			}
			bmpStream.seekg(padding, std::ios::cur);
		}
	}
	Texture2D(const std::string fileName)
	{
		*this = Texture2D(fileName.c_str());
	}
	Texture2D(int width, int height)
		:
		width(width),
		height(height)
	{
		pixels = new Color[width*height];
	}
	Texture2D(const Texture2D& other)
	{
		*this = other;
	}
	Texture2D& operator=(const Texture2D& other)
	{
		delete[] pixels;
		pixels = nullptr;
	
		this->width = other.width;
		this->height = other.height;

		const int size = width * height;
		this->pixels = new Color[size];

		for (int i = 0; i < size; i++)
		{
			pixels[i] = other.pixels[i];
		}

		return *this;
	}
	~Texture2D()
	{
		delete [] pixels;
		pixels = nullptr;
	}
	void SetPixel(int x, int y, Color c)
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);

		pixels[x + y * width] = c;
	}
	Color GetPixel(int x, int y) const
	{
		assert(x >= 0);
		assert(x < width);
		assert(y >= 0);
		assert(y < height);

		return pixels[x + y * width];
	}
	int GetWidth() const
	{
		return width;
	}
	int GetHeight() const
	{
		return height;
	}
};