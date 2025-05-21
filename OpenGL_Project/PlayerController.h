#pragma once
#include "Components.h"

class PlayerController : public Component
{
private:
	float objectMoveSpeed = 3.f;
	float cameraMoveSpeed = 15.f;

	glm::vec3 velocity;


	float distance;
	float height;

	glm::vec3 position;
	glm::vec3 targetPosition;
	float t;

	bool texToggle;


	bool mouseCursorVisibilityToggleLock;
	bool mouseCursorVisibility;

	bool wireframeToggleLock;
	bool wireframeMode;

	bool cameraToggleLock;
	bool cameraToggle;

	int tex0;
	int tex1;

public:
	PlayerController(float _objectMoveSpeed, float _cameraMoveSpeed, float _distance, int _altTex, int _altTex1);
	~PlayerController();

	void HandleStates();
	void Update()override;

	void ToggleTexture();
};

