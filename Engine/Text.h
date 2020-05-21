#pragma once

#include "Graphics.h"
#include "Texture2D.h"
#include "Vec2.h"

class Text
{
public:
	Text(const std::string& filename, Color chroma = Colors::White);
	void Draw(const std::string& text, const Vei2& pos, Color color, Graphics& gfx) const;
private:
private:
	// holds the font sheet bitmap data
	Texture2D surface;
	// this gives the dimensions of a glyph in the font sheet
	int glyphWidth;
	int glyphHeight;
	// number of rows / columns in the font sheet (this is fixed)
	static constexpr int nColumns = 32;
	static constexpr int nRows = 3;
	// font sheet chroma color
	Color chroma;
	// start and end drawable character codes
	static constexpr char firstChar = ' ';
	static constexpr char lastChar = '~';
};