#pragma once
#include "Components.h"

class PlayerController : public Component
{
protected:
	float mouseSensitivity = 3.f;
	float cameraMoveSpeed = 15.f;

	glm::vec3 velocity;

	float rotX;
	float rotY;


	glm::vec2 prevMousePos = glm::vec2(0.0f, 0.0f);
	glm::vec2 mousePos = glm::vec2(0.0f, 0.0f);

public:
	PlayerController(float _mouseSensitivity, float _cameraMoveSpeed);
	~PlayerController();

	void Update()override;
};

