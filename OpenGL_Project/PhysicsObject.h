#pragma once
#include "Collider.h"


class PhysicsObject : public Component 
{
public:
	PhysicsObject();
	~PhysicsObject();

	void Init()override;

	glm::vec3 velocity;

	Collider* collider;
};