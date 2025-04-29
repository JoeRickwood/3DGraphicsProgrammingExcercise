#pragma once
#include "Components.h"

class PlayerController : public Component
{
private:
	float moveSpeed = 3.f;
	float height = 2.f;
	float jumpHeight = 5.f;
	float mouseSensitivity = 3.f;

	float jumpResetTimer;

	glm::vec2 mousePos;
	glm::vec2 prevMousePos;

public:
	PlayerController(float _moveSpeed = 3.f, float _height = 2.f, float _mouseSensitivity = 3.f);
	~PlayerController();

	void Update()override;
	void Jump();
};

