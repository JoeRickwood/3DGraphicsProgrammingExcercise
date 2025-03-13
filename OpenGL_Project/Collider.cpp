#include "Collider.h"
#include "Physics.h"

Collider::Collider(glm::vec2 _size)
{
	size = _size;

	Physics::Instance().AddCollider(this);

	rect = Rect();
}

Collider::~Collider()
{
}

void Collider::Update()
{
	UpdateRect();
}

void Collider::UpdateRect()
{
	rect.top = parent->position.y + (size.y * 0.5f * parent->scale.y);
	rect.bottom = parent->position.y - (size.y * 0.5f * parent->scale.y);
	rect.right = parent->position.x + (size.x * 0.5f * parent->scale.x);
	rect.left = parent->position.x - (size.x * 0.5f * parent->scale.x);

	rect.width = size.x * parent->scale.x;
	rect.height = size.y * parent->scale.y;
}

Rect* Collider::GetRect()
{
	return &rect;
}
