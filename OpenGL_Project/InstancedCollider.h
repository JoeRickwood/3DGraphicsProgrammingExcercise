#pragma once
#include "Collider.h"

class InstancedCollider : public Component
{
private:
	std::vector<Collider*> instances;

public:
	InstancedCollider();
	~InstancedCollider();

	void AddInstance(glm::vec3 size, glm::vec3 offset);

	bool Contains(glm::vec3 point);
	bool Intersects(Collider* col);
};

