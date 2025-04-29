#pragma once
#include "Components.h"

class Collider : public Component
{
public:
	Collider(glm::vec3 _size, glm::vec3 _offset);
	~Collider();

	glm::vec3 size;
	glm::vec3 offset;

	bool Contains(glm::vec3 point);
};



