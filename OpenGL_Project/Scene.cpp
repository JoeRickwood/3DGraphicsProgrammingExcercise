#include "Scene.h"
#include "Time.h"

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


	//Do Physics After Update Happens

	//Check All Physics Objects
	for (int i = 0; i < objects.size(); i++)
	{
		auto obj = objects[i]->GetComponent<PhysicsObject>();

		if (obj == NULL) 
		{
			continue;
		}

		//Resolve X Collisions
		float prevX = obj->parent->position.x;
		obj->parent->position.x += obj->velocity.x * Time::Instance().deltaTime;

		if (CheckCollision(obj->parent->position)) 
		{
			obj->velocity.x = 0.f;
			obj->parent->position.x = prevX;
		}

		//Resolve Y Collisions
		float prevY = obj->parent->position.y;
		obj->parent->position.y += obj->velocity.y * Time::Instance().deltaTime;

		if (CheckCollision(obj->parent->position))
		{
			obj->velocity.y = 0.f;
			obj->parent->position.y = prevY;
		}

		//Resolve Z Collisions
		float prevZ = obj->parent->position.z;
		obj->parent->position.z += obj->velocity.z * Time::Instance().deltaTime;

		if (CheckCollision(obj->parent->position))
		{
			obj->velocity.z = 0.f;
			obj->parent->position.z = prevZ;
		}
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

bool Scene::CheckCollision(glm::vec3 _position)
{
	for (int i = 0; i < objects.size(); i++)
	{
		auto obj = objects[i]->GetComponent<Collider>();

		if (obj == NULL)
		{
			continue;
		}

		if (obj->Contains(_position)) 
		{
			return true;
		}
	}

	return false;
}
