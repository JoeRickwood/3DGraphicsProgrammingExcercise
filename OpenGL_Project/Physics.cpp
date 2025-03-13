#include "Physics.h"

void Physics::ResolveCollisions()
{
	for (PhysicsObject* obj : objects)
	{
		if (obj->collider == nullptr) 
		{
			continue;
		}

		Rect* A = obj->collider->GetRect();

		//Resolve X collisions
		float prevX = obj->parent->position.x;
		obj->parent->position.x += obj->velocity.x * Time::Instance().deltaTime;
		obj->collider->UpdateRect();

		for (int i = 0; i < colliders.size(); i++)
		{
			if (obj->collider == colliders[i])
			{
				continue;
			}

			Rect* B = colliders[i]->GetRect();

			if(!A->Intersects(B))
			{
				continue;
			}

			obj->velocity.x = 0;
			obj->parent->position.x = prevX;
			obj->collider->UpdateRect();
		}


		//Resolve Y Collisions
		float prevY = obj->parent->position.y;
		obj->parent->position.y += obj->velocity.y * Time::Instance().deltaTime;
		obj->collider->UpdateRect();

		for (int i = 0; i < colliders.size(); i++)
		{
			if (obj->collider == colliders[i])
			{
				continue;
			}

			Rect* B = colliders[i]->GetRect();

			if (!A->Intersects(B))
			{
				continue;
			}

			obj->velocity.y = 0;
			obj->parent->position.y = prevY;
			obj->collider->UpdateRect();
		}
	}
}

void Physics::AddCollider(Collider* obj)
{
	colliders.push_back(obj);
}

void Physics::RemoveCollider(Collider* obj)
{
	for (int i = 0; i < colliders.size(); i++)
	{
		if (colliders[i] == obj)
		{
			colliders.erase(colliders.begin() + i);
			return;
		}
	}
}

void Physics::AddObject(PhysicsObject* obj)
{
	objects.push_back(obj);
}

void Physics::RemoveObject(PhysicsObject* obj)
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i] == obj)
		{
			objects.erase(objects.begin() + i);
			return;
		}
	}
}

bool Physics::CheckPoint(glm::vec3 _point)
{
	for (int i = 0; i < colliders.size(); i++)
	{
		if (colliders[i]->GetRect()->Contains(_point))
		{
			return true;
		}
	}

	return false;
}

Physics::Physics()
{
}


Physics::~Physics()
{
}
