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
	for (int i = 0; i < objects.size(); ++i)
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

void Scene::ShaderInit()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr)
		{
			continue;
		}

		obj->ShaderInit();
	}
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

	Time::Instance().BeginTimer("Physics");
	HandlePhysics();
	Time::Instance().EndTimer();
}

void Scene::Render()
{
	glViewport(0, 0, (GLsizei)GraphicsLoader::Instance().windowSize.x, (GLsizei)GraphicsLoader::Instance().windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Render();
	}
}

void Scene::ShaderUpdate()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->ShaderUpdate();
	}
}

void Scene::HandlePhysics()
{
	//Check All Physics Objects
	for (int i = 0; i < objects.size(); ++i)
	{
		auto obj = objects[i]->GetComponent<PhysicsObject>();
		auto col = objects[i]->GetComponent<Collider>();

		if (obj == NULL)
		{
			continue;
		}

		//Resolve X Collisions
		float prevX = obj->parent->position.x;
		obj->parent->position.x += obj->velocity.x * Time::Instance().deltaTime;

		if (CheckCollision(col))
		{
			obj->velocity.x = 0.f;
			obj->parent->position.x = prevX;
		}

		//Resolve Y Collisions
		float prevY = obj->parent->position.y;
		obj->parent->position.y += obj->velocity.y * Time::Instance().deltaTime;

		if (CheckCollision(col))
		{
			obj->velocity.y = 0.f;
			obj->parent->position.y = prevY;
		}

		//Resolve Z Collisions
		float prevZ = obj->parent->position.z;
		obj->parent->position.z += obj->velocity.z * Time::Instance().deltaTime;

		if (CheckCollision(col))
		{
			obj->velocity.z = 0.f;
			obj->parent->position.z = prevZ;
		}
	}
}

bool Scene::CheckCollision(Collider* col)
{
	if (col == nullptr) 
	{
		return false;
	}

	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i] == col->parent)
		{
			continue;
		}

		auto obj = objects[i]->GetComponent<Collider>();
		auto instancedCol = objects[i]->GetComponent<InstancedCollider>();

		if (obj != NULL)
		{
			if (obj->Intersects(col))
			{
				return true;
			}
		}

		if (instancedCol != NULL)
		{
			if (instancedCol->Intersects(col))
			{
				return true;
			}
		}
	}

	return false;
}

Collider* Scene::OverlapPoint(glm::vec3 point)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		auto obj = objects[i]->GetComponent<Collider>();

		if (obj == NULL) 
		{
			continue;
		}

		if(obj->Contains(point)) 
		{
			return obj;
		}
	}

	return nullptr;
}