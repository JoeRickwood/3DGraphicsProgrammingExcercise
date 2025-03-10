#pragma once
#include "Renderer.h"

class Button : public Component
{
private:
	std::vector<void(*)()> listeners;
	bool presslock;

public:
	Button();
	~Button();

	void Update()override;

	void AddListener(void(*func)());
	void Click();
};