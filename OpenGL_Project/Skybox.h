#pragma once
#include "ObjectInstance.h"
#include "MeshLoader.h"

class Skybox : public Component
{
private:
	Mesh* mesh;
	int skyboxShader;
	int skyboxTexID;

public:
	Skybox(int _skybox, int _shader);
	~Skybox();

	void Render()override;
};

