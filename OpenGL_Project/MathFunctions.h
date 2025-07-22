#pragma once


static float Clamp(float _val, float _min, float _max)
{
	if (_val < _min) {
		_val = _min;
	}

	if (_val > _max) {
		_val = _max;
	}

	return _val;
}

static float Lerp(float a, float b, float t)
{
	return ((b - a) * t) + a;
}

static float InverseLerp(float a, float b, float value) 
{
	return Clamp((value - a) / (b - a), 0.0f, 1.0f);
}

static glm::vec3 Lerp(glm::vec3 a, glm::vec3 b, float t)
{
	return glm::vec3
	(
		Lerp(a.x, b.x, t),
		Lerp(a.y, b.y, t),
		Lerp(a.z, b.z, t)
	);
}


static double RandomValue(int _X, int _Y, int _seed = 743927492794)
{
	double value = 0.0;

	int noiseVal = _X + _Y * _seed;
	noiseVal = (noiseVal << 13) ^ noiseVal;

	int T = (noiseVal * (noiseVal * noiseVal * 15731 + 789221) + 1376312589) & 0x7fffffff;
	value = 1.0f - (double)T * 0.93132257461548515625e-9;
	return value;
}

static double CosineInterpolate(double a, double b, double t) 
{
	double t2 = (1.0 - cos(t * 3.14159265)) / 2.0;
	return (a * (1.0 - t2) + b * t2);
}

static double Smooth(int X, int Y)
{
	double corners = (RandomValue(X - 1, Y - 1) + RandomValue(X + 1, Y - 1) + RandomValue(X - 1, Y + 1) + RandomValue(X + 1, Y + 1)) / 16.0f;
	double sides = (RandomValue(X, Y - 1) + RandomValue(X, Y + 1) + RandomValue(X - 1, Y) + RandomValue(X + 1, Y)) / 8.0f;
	double center = RandomValue(X, Y) / 4.0f;

	return corners + sides + center;
}

static double SmoothedInterpolate(double X, double Y) 
{
	int truncX = (int)X;
	int truncY = (int)Y;

	double fractX = X - (double)truncX;
	double fractY = Y - (double)truncY;

	double v1 = Smooth(truncX, truncY);
	double v2 = Smooth(truncX + 1, truncY);
	double v3 = Smooth(truncX, truncY + 1);
	double v4 = Smooth(truncX + 1, truncY + 1);

	double interpolate1 = CosineInterpolate(v1, v2, fractX);
	double interpolate2 = CosineInterpolate(v3, v4, fractX);

	double final = CosineInterpolate(interpolate1, interpolate2, fractY);
	return final;
}

static double TotalNoisePerPoint(float _x, float _y) 
{
	int octaves = 20;
	float wavelength = 128.0f;
	float gain = 0.5f;
	float lacunarity = 2.0f;

	float maxValue = 0.0f;
	double total = 0.0f;

	for (int i = 0; i < octaves; i++)
	{
		float freq = (float)pow(lacunarity, i) / wavelength;
		float amplitude = (float)pow(gain, i);
		maxValue += amplitude;

		total += SmoothedInterpolate(_x * freq, _y * freq) * amplitude;
	}

	return total / maxValue;
}

static double PerlinNoise(float _x, float _y)
{
	return TotalNoisePerPoint(_x, _y);
}




