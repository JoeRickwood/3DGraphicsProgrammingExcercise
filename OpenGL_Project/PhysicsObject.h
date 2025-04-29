#pragma once
#include "Components.h" 

class PhysicsObject : public Component
{
private:

public:
	glm::vec3 velocity;

	PhysicsObject();
	~PhysicsObject();
};

