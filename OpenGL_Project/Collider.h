#pragma once
#include "ObjectInstance.h"

struct Rect 
{
public:
	float top;
	float bottom;
	float left;
	float right;

	float height;
	float width;


	bool Contains(glm::vec3 point)
	{
		if (point.x > left && point.x < right &&
			point.y > bottom && point.y < top) 
		{
			return true;
		}

		return false;
	};

	bool Intersects(Rect* comp) 
	{
		if (left < comp->right && right > comp->left &&
			top > comp->bottom && bottom < comp->top) {
			return true;
		}

		return false;
	};
};

class Collider : public Component 
{ 
public:
	Collider(glm::vec2 _size = glm::vec2(1.f, 1.f));
	~Collider();

	void Update()override;

	void UpdateRect();

	Rect* GetRect();
private:
	glm::vec2 size;
	Rect rect;
};