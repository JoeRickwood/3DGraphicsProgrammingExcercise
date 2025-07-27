#pragma once
#include "ObjectInstance.h"
#include <functional>

class Button : public Component
{
protected:
	bool mouseOver;
	bool pressed;

	glm::vec2 mousePos;
	glm::vec4 color;

	std::vector<std::function<void()>> listeners;

	bool pressLock = false;

	const bool Intersects(glm::vec3 _position) const;

public:
	Button();
	~Button();

	void Update()override;
	void AddListener(std::function<void()> _func);
	void Click();

	glm::vec4 GetColor();
	void SetColor(glm::vec4 _color);
};

