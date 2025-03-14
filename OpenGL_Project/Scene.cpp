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

void Scene::RemoveObject(ObjectInstance* _obj)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == _obj)
		{
			objects.erase(objects.begin() + i);
			return;
		}
	}
}

ObjectInstance* Scene::FindObject(std::string _name)
{
	for (auto& obj : objects)
	{
		if (obj == nullptr)
		{
			continue;
		}

		if (obj->name == _name) 
		{
			return obj;
		}
	}

	return nullptr;
}

void Scene::Update()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr) 
		{
			continue;
		}

		obj->Update();
	}
}

void Scene::Render()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr)
		{
			continue;
		}

		obj->Render();
	}
}

void Scene::ShaderUpdate()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr)
		{
			continue;
		}

		obj->ShaderUpdate();
	}
}
