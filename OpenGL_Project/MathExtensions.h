#pragma once
#include "AssetLoader.h"


//Linear Interpolation
//Finds A Value Between Two Points On A Fractional Value '_t' In 1D Space
static float Lerp(float _a, float _b, float _t)
{
	return ((_b - _a) * _t) + _a;
}

//Finds A Value Between Two Points On A Fractional Value '_t' In 3D Space
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

static float Smoothstep(float _value, float _edge0, float _edge1) 
{
	float t = glm::clamp((_value - _edge0) / (_edge1 - _edge0), 0.0f, 1.0f);

	return t * t * (3.0f - 2.0f * t);
}


//Easing Functions Have Been Ported From Easings.net, Originally Programmed In TypeScript
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




