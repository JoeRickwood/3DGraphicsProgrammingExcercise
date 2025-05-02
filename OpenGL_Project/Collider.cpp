#include "Collider.h"
#include <iostream>

bool Collider::Overlaps1D(float firstMinX, float firstMaxX, float secondMinX, float secondMaxX)
{
	return firstMinX < secondMaxX && secondMinX < firstMaxX;
}

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

bool Collider::Intersects(Collider* col)
{
	bool xAxis = Overlaps1D(col->parent->position.x - (col->size.x / 2.f) + col->offset.x, col->parent->position.x + (col->size.x / 2.f) + col->offset.x, parent->position.x - (size.x / 2.f) + offset.x, parent->position.x + (size.x / 2.f) + offset.x);
	bool yAxis = Overlaps1D(col->parent->position.y - (col->size.y / 2.f) + col->offset.y, col->parent->position.y + (col->size.y / 2.f) + col->offset.y, parent->position.y - (size.y / 2.f) + offset.y, parent->position.y + (size.y / 2.f) + offset.y);
	bool zAxis = Overlaps1D(col->parent->position.z - (col->size.z / 2.f) + col->offset.z, col->parent->position.z + (col->size.x / 2.f) + col->offset.z, parent->position.z - (size.z / 2.f) + offset.z, parent->position.z + (size.z / 2.f) + offset.z);

	return xAxis and yAxis and zAxis;
}
