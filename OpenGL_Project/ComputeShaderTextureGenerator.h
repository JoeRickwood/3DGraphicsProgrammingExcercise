#pragma once
#include "ObjectInstance.h"

class ComputeShaderTextureGenerator : public Component
{
protected:
	std::string shaderKey;
	GLuint textureOutput;

	const int textureWidth = 512;
	const int textureHeight = 512;

public:
	ComputeShaderTextureGenerator(std::string _computeShaderKey);
	~ComputeShaderTextureGenerator();

	void GenerateTexture();
};

