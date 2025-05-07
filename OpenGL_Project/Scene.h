#pragma once
#include "ObjectInstance.h"
#include "Components.h"
#include "Skybox.h"

class Scene
{
public:
	static Scene& Current() { static Scene scene; return scene; }

	Scene();
	~Scene();

	void AddObject(ObjectInstance* _obj);
	void RemoveObject(ObjectInstance* _obj);
	ObjectInstance* FindObject(std::string _name);

	void ShaderInit();
	void Update();
	void Render();
	void ShaderUpdate();
	void HandlePhysics();

	bool CheckCollision(Collider* col);
	Collider* OverlapPoint(glm::vec3 point);

private:
	std::vector<ObjectInstance*> objects;
};

