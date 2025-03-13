#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddObject(ObjectInstance* _obj)
{
	objects.push_back(_obj);
}

void Scene::Update()
{
	for (auto& obj : objects)
	{
		obj->Update();
	}
}

void Scene::Render()
{
	for (auto& obj : objects)
	{
		obj->Render();
	}
}

void Scene::ShaderUpdate()
{
	for (auto& obj : objects)
	{
		obj->ShaderUpdate();
	}
}
