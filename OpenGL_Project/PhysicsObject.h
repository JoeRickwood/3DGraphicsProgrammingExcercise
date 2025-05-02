#pragma once
#include "ObjectInstance.h" 

class PhysicsObject : public Component
{
private:

public:
	glm::vec3 velocity;

	PhysicsObject();
	~PhysicsObject();
};

