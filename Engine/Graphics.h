/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Vec2.h"
#include "Texture2D.h"

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame();
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c );
	void DrawLine(int x0, int y0, int x1, int y1, Color c)
	{
		float m = 0.0f;
		if (x1 != x0)
		{
			m = float((y1 - y0) / (x1 - x0));
		}

		if (x1 != x0 && std::abs(m) <= 1.0f)
		{
			if (x0 > x1)
			{
				std::swap(x0, x1);
				std::swap(y0, y1);
			}

			const float b = y0 - m * x0;

			for (int x = (int)x0; x < (int)x1; x++)
			{
				const float y = m * (float)x + b;
				PutPixel(x, (int)y, c);
			}
		}
		else
		{
			if (y0 > y1)
			{
				std::swap(x0, x1);
				std::swap(y0, y1);
			}

			const float w = float((x1 - x0) / (y1 - y0));
			const float p = x0 - w * y0;

			for (int y = (int)y0; y < (int)y1; y++)
			{
				const float x = w * (float)y + p;
				PutPixel((int)x, y, c);
			}
		}
	}
	void DrawLine(Vec2 p0, Vec2 p1, Color c)
	{
		float m = 0.0f;
		if (p1.x != p0.x)
		{
			m = (p1.y - p0.y) / (p1.x - p0.x);
		}

		if (p1.x != p0.x && std::abs(m) <= 1.0f)
		{
			if (p0.x > p1.x)
			{
				std::swap(p0, p1);
			}

			const float b = p0.y - m * p0.x;

			for (int x = (int)p0.x; x < (int)p1.x; x++)
			{
				const float y = m * (float)x + b;
				PutPixel(x, (int)y, c);
			}
		}
		else
		{
			if (p0.y > p1.y)
			{
				std::swap(p0, p1);
			}

			const float w = (p1.x - p0.x) / (p1.y - p0.y);
			const float p = p0.x - w * p0.y;

			for (int y = (int)p0.y; y < (int)p1.y; y++)
			{
				const float x = w * (float)y + p;
				PutPixel((int)x, y, c);
			}
		}
	}
	void DrawRectangle(int x0, int y0, int x1, int y1, Color c)
	{
		if (x0 > x1)
		{
			std::swap(x0, x1);
		}
		if (y0 > y1)
		{
			std::swap(y0, y1);
		}

		for (int j = y0; j < y1; j++)
		{
			for (int i = x0; i < x1; i++)
			{
				if (i >= 0 && i < ScreenWidth && j >= 0 && j < ScreenHeight)
				{
					PutPixel(i, j, c);
				}
			}
		}
	}
	void DrawRectangle(Vec2 p0, Vec2 p1, Color c)
	{
		if (p0.x > p1.x)
		{
			std::swap(p0, p1);
		}
		if (p0.y > p1.y)
		{
			std::swap(p0, p1);
		}

		for (int j = int(p0.y); j < int(p1.y); j++)
		{
			for (int i = int(p0.x); i < int(p1.x); i++)
			{
				if (i >= 0 && i < ScreenWidth && j >= 0 && j < ScreenHeight)
				{
					PutPixel(i, j, c);
				}
			}
		}
	}
	void DrawTexture(int x, int y, const Texture2D& t)
	{
		DrawTexture(x, y, 0, 0, t.GetWidth(), t.GetHeight(), t);
	}
	void DrawTexture(int x, int y, int x0, int y0, int x1, int y1, const Texture2D& t)
	{
		assert(x0 >= 0);
		assert(x1 <= t.GetWidth());
		assert(y0 >= 0);
		assert(y1 <= t.GetHeight());

		for (int j = y0; j < y1; j++)
		{
			for (int i = x0; i < x1; i++)
			{
				PutPixel(i + x - x0, j + y - y0, t.GetPixel(i, j));
			}
		}
	}
	void DrawTexture(int x, int y, int x0, int y0, int x1, int y1, int cx0, int cy0, int cx1, int cy1, const Texture2D& t)
	{
		if (x < cx0)
		{
			x0 += cx0 - x;
			x = cx0;
		}
		if (y < cy0)
		{
			y0 += cy0 - y;
			y = cy0;
		}
		if (x + x1 - x0 > cx1)
		{
			x1 -= x + x1 - x0 - cx1;
		}
		if (y + y1 - y0 > cy1)
		{
			y1 -= y + y1 - y0 - cy1;
		}
		DrawTexture(x, y, x0, y0, x1, y1, t);
	}
	void DrawTextureChroma(int x, int y, const Texture2D& t, Color chroma)
	{
		DrawTextureChroma(x, y, 0, 0, t.GetWidth(), t.GetHeight(), t, chroma);
	}
	void DrawTextureChroma(int x, int y, int x0, int y0, int x1, int y1, const Texture2D& t, Color chroma)
	{
		assert(x0 >= 0);
		assert(x1 <= t.GetWidth());
		assert(y0 >= 0);
		assert(y1 <= t.GetHeight());

		for (int j = y0; j < y1; j++)
		{
			for (int i = x0; i < x1; i++)
			{
				if (t.GetPixel(i, j) != chroma)
				{
					PutPixel(i + x - x0, j + y - y0, t.GetPixel(i, j));
				}
			}
		}
	}
	void DrawTextureChroma(int x, int y, int x0, int y0, int x1, int y1, int cx0, int cy0, int cx1, int cy1, const Texture2D& t, Color chroma)
	{
		if (x < cx0)
		{
			x0 += cx0 - x;
			x = cx0;
		}
		if (y < cy0)
		{
			y0 += cy0 - y;
			y = cy0;
		}
		if (x + x1 - x0 > cx1)
		{
			x1 -= x + x1 - x0 - cx1;
		}
		if (y + y1 - y0 > cy1)
		{
			y1 -= y + y1 - y0 - cy1;
		}
		DrawTextureChroma(x, y, x0, y0, x1, y1, t, chroma);
	}
	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
};