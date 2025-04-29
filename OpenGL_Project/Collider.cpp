#include "Collider.h"

Collider::Collider(glm::vec3 _size, glm::vec3 _offset)
{
	size = _size;
	offset = _offset;
}

Collider::~Collider()
{
}

bool Collider::Contains(glm::vec3 point)
{
	if (point.x > parent->position.x - (size.x * 0.5f) + offset.x && point.x < parent->position.x + (size.x * 0.5f) + offset.x &&
		point.y > parent->position.y - (size.y * 0.5f) + offset.y && point.y < parent->position.y + (size.y * 0.5f) + offset.y &&
		point.z > parent->position.z - (size.z * 0.5f) + offset.z && point.z < parent->position.z + (size.z * 0.5f) + offset.z)
	{
		return true;
	}


	return false;
}
