#pragma once
#include "ObjectInstance.h"

class CameraController : public Component
{
public:
	float moveSpeed;
	float distance;

	CameraController(float _moveSpeed, float _distance);
	~CameraController();

	void Init()override;
	void Update()override;

private:
	glm::vec3 position;
	glm::vec3 targetPosition;
	float t;
};

