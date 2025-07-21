#pragma once
#include "AssetLoader.h"

static float Lerp(float a, float b, float t)
{
	return ((b - a) * t) + a;
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




