#include "Scene.h"
#include "Time.h"

void Scene::SetAmbientLightStength(float _strength)
{
	ambientStrength = _strength;
}

void Scene::SetAmbientLightColor(glm::vec3 _color)
{
	ambientColor = _color;
}

const glm::vec3 Scene::GetAmbientLight()
{
	return ambientColor * ambientStrength;
}

Scene::Scene()
{
	ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientStrength = 0.2f;
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
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i] == _obj)
		{
			objects.erase(objects.begin() + i);
			return;
		}
	}
}	

std::vector<ObjectInstance*> Scene::GetAllObjects()
{
	return objects;
}

ObjectInstance* Scene::FindObject(std::string _name)
{
	for (auto& obj : objects)
	{
		if (obj->GetName() == _name)
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