#pragma once
#include "ObjectInstance.h"
#include "PhysicsObject.h"

class Physics
{
public:
	static Physics& Instance() { static Physics physics; return physics; }

	void ResolveCollisions();

	void AddCollider(Collider* obj);
	void RemoveCollider(Collider* obj);

	void AddObject(PhysicsObject*);
	void RemoveObject(PhysicsObject*);

	bool CheckPoint(glm::vec3 _point);

private:
	Physics();
	~Physics();

	std::vector<PhysicsObject*> objects;
	std::vector<Collider*> colliders;
};

