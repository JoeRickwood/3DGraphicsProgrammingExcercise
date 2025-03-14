#pragma once
#include "ObjectInstance.h"
#include "Components.h"

class Scene
{
public:
	static Scene& Current() { static Scene scene; return scene; }

	Scene();
	~Scene();

	void AddObject(ObjectInstance* _obj);
	void RemoveObject(ObjectInstance* _obj);
	ObjectInstance* FindObject(std::string _name);

	void Update();
	void Render();
	void ShaderUpdate();

private:

	std::vector<ObjectInstance*> objects;
};

