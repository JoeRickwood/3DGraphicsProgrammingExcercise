#pragma once
#include "Mesh.h"

class MeshLoader
{
public:
	static MeshLoader& Instance() { static MeshLoader shaderLoader; return shaderLoader; }

	std::vector<Mesh*> meshes;

	void Init();
	void LinkMeshes();
	Mesh* GetMesh(int _type);

	void LoadMesh(std::string filepath);

private:
	MeshLoader();
	~MeshLoader();
};
