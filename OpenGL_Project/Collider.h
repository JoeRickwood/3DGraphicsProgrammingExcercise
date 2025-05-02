#pragma once
#include "ObjectInstance.h"

class Collider : public Component
{
private:
	bool Overlaps1D(float firstMinX, float firstMaxX, float secondMinX, float secondMaxX);

public:
	Collider(glm::vec3 _size, glm::vec3 _offset);
	~Collider();

	glm::vec3 size;
	glm::vec3 offset;

	virtual bool Contains(glm::vec3 point);
	virtual bool Intersects(Collider* col);
};



