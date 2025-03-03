#pragma once
#include "RenderableInstance.h"


class InputMove : public Component
{
private:
	float inputX;
	float inputY;
	float rotationInput;
	float scaleInput;

public:
	InputMove();
	~InputMove();

	void Update()override;
};
