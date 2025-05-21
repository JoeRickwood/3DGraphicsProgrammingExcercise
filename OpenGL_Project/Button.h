#pragma once
#include "ObjectInstance.h"
#include <functional>

class Button : public Component
{
protected:
	bool mouseOver;

	int textureID0;
	int textureID1;

	glm::vec2 mousePos;

	bool debugToggleLock;
	bool debugToggle;

	std::vector<std::function<void()>> listeners;

	bool pressLock = false;

	const bool Intersects(glm::vec3 _position) const;

public:
	void DebugPos();

	Button(int _texID0, int _texID1);
	~Button();

	void Update()override;
	void AddListener(std::function<void()> _func);
	void Click();
};

