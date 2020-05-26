#pragma once

#include "Graphics.h"

namespace SpriteEffects
{
	struct NoEffect
	{
		void operator()(Graphics& gfx, int x, int y, Color c)
		{
			gfx.PutPixel(x, y, c);
		}
	};
	struct Chroma
	{
		Chroma(Color c)
			:
			chroma(c)
		{

		}
		void operator()(Graphics& gfx, int x, int y, Color c) const
		{
			if (c != chroma)
			{
				gfx.PutPixel(x, y, c);
			}
		}
	private:
		Color chroma;
	};
	struct Substitute
	{
		Substitute(Color c, Color s)
			:
			chroma(c),
			substitute(s)
		{

		}
		void operator()(Graphics& gfx, int x, int y, Color c)
		{
			if (c != chroma)
			{
				gfx.PutPixel(x, y, substitute);
			}
		}
	private:
		Color chroma;
		Color substitute;
	};
	struct Ghost
	{
		Ghost(Color c)
			:
			chroma(c)
		{

		}
		void operator()(Graphics& gfx, int x, int y, Color c)
		{
			if (c != chroma)
			{
				const Color sColor(gfx.GetPixel(x, y));
				gfx.PutPixel(x, y, Color{ unsigned char((sColor.GetR() + c.GetR()) / 2),
					unsigned char((sColor.GetG() + c.GetG()) / 2),
					unsigned char((sColor.GetB() + c.GetB()) / 2)});
			}
		}
	private:
		Color chroma;
	};
}