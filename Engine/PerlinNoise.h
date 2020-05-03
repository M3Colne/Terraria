//http://www.arendpeter.com/Perlin_Noise.html
#pragma once

#include <cmath>

class Noise
{
public:
	//Perlin Noise
	static float PerlinNoise_1D(float x) //This is a default perlin noise
	{
		float total = 0.0f;
		float denominator = 0.0f;
		float freq = 1.0f;
		float ampl = 256.0f;

		for (int i = 0; i < 4; i++)
		{
			total += InterpolatedNoise(x * freq) * ampl;
			denominator += 2 * ampl;
			freq *= 2;
			ampl /= 2;
		}

		//It gives values between 0 and a big number

		return total / denominator; //Now it gives values between 0 and 1
	}
	static float PerlinNoise_2D(float x, float y) //This is a default perlin noise
	{
		float total = 0.0f;
		float denominator = 0.0f;
		float freq = 1.0f;
		float ampl = 256.0f;

		for (int i = 0; i < 4; i++)
		{
			total += InterpolatedNoise(x * freq, y * freq) * ampl;
			denominator += 2 * ampl;
			freq *= 2;
			ampl /= 2;
		}

		return total / denominator;
	}
	static float PerlinNoise_1D(float x, float freq, float ampl, int nOctaves) //This is customazible perlin noise
	{
		float total = 0.0f;
		float denominator = 0.0f;

		for (int i = 0; i < nOctaves; i++)
		{
			total += InterpolatedNoise(x * freq) * ampl;
			denominator += 2 * ampl;
			freq *= 2;
			ampl /= 2;
		}

		//It gives values between 0 and a big number

		return total / denominator; //Now it gives values between 0 and 1
	}
	static float PerlinNoise_2D(float x, float y, float freq, float ampl, int nOctaves) //This is customazible perlin noise
	{
		float total = 0.0f;
		float denominator = 0.0f;

		for (int i = 0; i < nOctaves; i++)
		{
			total += InterpolatedNoise(x * freq, y * freq) * ampl;
			denominator += 2 * ampl;
			freq *= 2;
			ampl /= 2;
		}

		return total / denominator;
	}
	static float PerlinNoise_3D(float x, float y, float z)
	{
		float AB = PerlinNoise_2D(x, y);
		float BC = PerlinNoise_2D(y, z);
		float AC = PerlinNoise_2D(x, z);

		float BA = PerlinNoise_2D(y, x);
		float CB = PerlinNoise_2D(z, y);
		float CA = PerlinNoise_2D(z, x);

		float ABC = AB + BC + AC + BA + CB + CA;

		return ABC / 6.0f;
	}
private:
	//Noise
	static float IntNoise(int x)
	{
		x = (x << 13) ^ x;
		return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589)) / 1073741824);
	}
	static float IntNoise(int x, int y)
	{
		int n = x + y * 57;
		n = (n << 13) ^ n;
		return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589)) / 1073741824);
	}
	static float SmoothNoise(int x)
	{
		return IntNoise(x) / 2 + IntNoise(x - 1) / 4 + IntNoise(x + 1) / 4;
	}
	static float SmoothNoise(int x, int y)
	{
		float corners = (IntNoise(x - 1, y - 1) + IntNoise(x + 1, y - 1) + IntNoise(x - 1, y + 1) + IntNoise(x + 1, y + 1)) / 16;
		float sides = (IntNoise(x - 1, y) + IntNoise(x + 1, y) + IntNoise(x, y - 1) + IntNoise(x, y + 1)) / 8;
		float center = IntNoise(x, y) / 4;
		return corners + sides + center;
	}

	//Interpolation
	static float Cosine_Interpolate(float a, float b, float x)
	{
		float ft = x * 3.1415927f;
		float f = (1 - cos(ft)) * 0.5f;

		return  a * (1 - f) + b * f;
	}
	static float InterpolatedNoise(float x)
	{
		int integer_X = int(x);
		float fractional_X = x - integer_X;

		float v1 = SmoothNoise(integer_X);
		float v2 = SmoothNoise(integer_X + 1);

		return Cosine_Interpolate(v1, v2, fractional_X);
	}
	static float InterpolatedNoise(float x, float y)
	{
		int integer_X = int(x);
		float fractional_X = x - integer_X;

		int integer_Y = int(y);
		float fractional_Y = y - integer_Y;

		float v1 = SmoothNoise(integer_X, integer_Y);
		float v2 = SmoothNoise(integer_X + 1, integer_Y);
		float v3 = SmoothNoise(integer_X, integer_Y + 1);
		float v4 = SmoothNoise(integer_X + 1, integer_Y + 1);

		float i1 = Cosine_Interpolate(v1, v2, fractional_X);
		float i2 = Cosine_Interpolate(v3, v4, fractional_X);

		return Cosine_Interpolate(i1, i2, fractional_Y);
	}
};