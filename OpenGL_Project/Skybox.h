#pragma once
#include "Renderer.h"
#include "AssetLoader.h"

class Skybox : public Renderer
{
protected:
	Mesh* mesh;

public:
	Skybox(std::string _skyboxShaderKey, std::string _skyboxTextureKey);
	~Skybox();

	void Init()override;

	void Render()override;
};

