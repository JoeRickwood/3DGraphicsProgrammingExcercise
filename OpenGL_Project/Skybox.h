#pragma once
#include "ObjectInstance.h"
#include "AssetLoader.h"

class Skybox : public Component
{
protected:
	Mesh* mesh;
	std::string skyboxShaderKey;
	std::string skyboxTextureKey;

public:
	Skybox(std::string _skyboxShaderKey, std::string _skyboxTextureKey);
	~Skybox();

	void Render()override;
};

