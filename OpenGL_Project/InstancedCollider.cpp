#include "InstancedCollider.h"

InstancedCollider::InstancedCollider()
{

}

InstancedCollider::~InstancedCollider()
{

}

void InstancedCollider::AddInstance(glm::vec3 size, glm::vec3 offset)
{
	Collider* col = new Collider(size, offset);
	col->parent = parent;

	instances.push_back(col);
}

bool InstancedCollider::Contains(glm::vec3 point)
{
	for (int i = 0; i < instances.size(); i++)
	{
		if (instances[i]->Contains(point)) 
		{
			return true;
		}
	}

	return false;
}

bool InstancedCollider::Intersects(Collider* col)
{
	for (int i = 0; i < instances.size(); i++)
	{
		if (instances[i]->Intersects(col))
		{
			return true;
		}
	}

	return false;
}
