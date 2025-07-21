#pragma once
#include "ObjectInstance.h"
#include <functional>

class Button : public Component
{
protected:
	bool mouseOver;

	glm::vec2 mousePos;

	std::vector<std::function<void()>> listeners;

	bool pressLock = false;

	const bool Intersects(glm::vec3 _position) const;

public:
	Button();
	~Button();

	void Update()override;
	void AddListener(std::function<void()> _func);
	void Click();
};

