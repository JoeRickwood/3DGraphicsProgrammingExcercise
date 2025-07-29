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

//Finds A Value Between Two Points On A Fractional Value '_t' In 2D Space
static glm::vec2 Lerp(glm::vec2 _a, glm::vec2 _b, float _t)
{
	return glm::vec2
	(
		Lerp(_a.x, _b.x, _t),
		Lerp(_a.y, _b.y, _t)
	);
}

//Finds A Value Between Two Points On A Fractional Value '_t' In 3D Space, Such As Positions
static glm::vec3 Lerp(glm::vec3 _a, glm::vec3 _b, float _t)
{
	return glm::vec3
	(
		Lerp(_a.x, _b.x, _t),
		Lerp(_a.y, _b.y, _t),
		Lerp(_a.z, _b.z, _t)
	);
}

//Finds A Value Between Two Points On A Fractional Value '_t' In 4D Space, Usually Used For Color
static glm::vec4 Lerp(glm::vec4 _a, glm::vec4 _b, float _t)
{
	return glm::vec4
	(
		Lerp(_a.x, _b.x, _t),
		Lerp(_a.y, _b.y, _t),
		Lerp(_a.z, _b.z, _t),
		Lerp(_a.w, _b.w, _t)
	);
}


static float InverseLerp(float a, float b, float value) 
{
	return Clamp((value - a) / (b - a), 0.0f, 1.0f);
}

static float Smoothstep(float _value, float _edge0, float _edge1)
{
	float t = glm::clamp((_value - _edge0) / (_edge1 - _edge0), 0.0f, 1.0f);

	return t * t * (3.0f - 2.0f * t);
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





//Easing Functions Have Been Ported From https://Easings.net, Originally Programmed In TypeScript
//https://www.typescriptlang.org

//Input Value Should Be Between 0 And 1, This Function Remaps Value To Be On A Cubic Ease-In Curve
static float EaseInExpo(float _value)
{
	return _value == 0.0f ? 0.0f : pow(2.0f, 10.0f * _value - 10.0f);
}

//Input Value Should Be Between 0 And 1, This Function Remaps Value To Be On A Cubic Ease-Out Curve
static float EaseOutExpo(float _value)
{
	return _value == 1.0f ? 1.0f : 1.0f - pow(2.0f, -10.0f * _value);
}

//Input Value Should Be Between 0 And 1, This Function Remaps Value To Be On A Cubic Ease-In + Ease-Out Curve
static float EaseInOutExpo(float _value)
{
	return _value == 0.0f ? 0.0f : _value == 1.0f ? 1.0f : _value < 0.5f ? pow(2.0f, 20.0f * _value - 10.0f) / 2.0f : (2.0f - pow(2.0f, -20.0f * _value + 10.0f)) / 2.0f;
}




