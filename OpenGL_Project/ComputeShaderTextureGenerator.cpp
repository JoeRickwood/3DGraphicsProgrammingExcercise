#include "ComputeShaderTextureGenerator.h"
#include <iostream>

ComputeShaderTextureGenerator::ComputeShaderTextureGenerator(std::string _computeShaderKey)
{
	shaderKey = _computeShaderKey;

	textureOutput = AssetLoader::CreateTextureComputeOutput(textureWidth, textureHeight);

	AssetLoader::Instance().AddTexture(textureOutput, "ComputeColor");
}

ComputeShaderTextureGenerator::~ComputeShaderTextureGenerator()
{

}

void ComputeShaderTextureGenerator::GenerateTexture()
{
	const int workGroupSize_X = 64;
	const int workGroupSize_y = 16;

	glUseProgram(AssetLoader::Instance().GetComputeShaderProgram(shaderKey));
	glBindImageTexture(0, textureOutput, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	glDispatchCompute(textureWidth / workGroupSize_X, textureHeight / workGroupSize_y, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	std::cout << AssetLoader::Instance().GetTexture("ComputeColor");
}
