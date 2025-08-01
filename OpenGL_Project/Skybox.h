#pragma once
#include "Renderer.h"
#include "AssetLoader.h"

class Skybox : public Renderer
{
protected:
	Mesh* mesh; //Skybox Mesh To use

public:
	Skybox(std::string _skyboxShaderKey, std::string _skyboxTextureKey);
	~Skybox();

	//Only Pass In The Shader Pass-ins We Need For The Skybox
	void InitializeRenderingInfo(GLuint program)override;
	void Init()override;

	void Render()override;
};

