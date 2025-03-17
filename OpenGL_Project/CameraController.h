#pragma once
#include "ObjectInstance.h"

class CameraController : public Component
{
public:
	float moveSpeed;

	CameraController(float _moveSpeed);
	~CameraController();

	void Init()override;
	void Update()override;

private:
	glm::vec3 input;


};

