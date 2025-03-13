#pragma once
#include "ObjectInstance.h"
#include "Components.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddObject(ObjectInstance* _obj);

	void Update();
	void Render();
	void ShaderUpdate();

private:
	std::vector<ObjectInstance*> objects;
};

