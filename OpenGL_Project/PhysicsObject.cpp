#include "PhysicsObject.h"
#include "Physics.h"

PhysicsObject::PhysicsObject()
{
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	Physics::Instance().AddObject(this);

	collider = nullptr;
}

PhysicsObject::~PhysicsObject()
{
	Physics::Instance().RemoveObject(this);
}

void PhysicsObject::Init()
{
	collider = parent->GetComponent<Collider>();
}
