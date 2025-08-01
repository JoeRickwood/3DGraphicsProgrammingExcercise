#pragma once
#include "Components.h"

class PlayerController : public Component
{
protected:
	float mouseSensitivity = 3.f;
	float cameraMoveSpeed = 15.f;

	//Stores X (Vertical) And Y (Horizontal) Rotation
	float rotX;
	float rotY;

	//Velocity Lerps Between Values To Give Smooth Movement
	glm::vec3 velocity;

	//Storing Mouse Position To Calculate Directional Mouse X and Y Vector
	glm::vec2 prevMousePos = glm::vec2(0.0f, 0.0f);
	glm::vec2 mousePos = glm::vec2(0.0f, 0.0f);

public:
	PlayerController(float _mouseSensitivity = 3.0f, float _cameraMoveSpeed = 10.0f);
	~PlayerController();

	void Update()override;

	//Getters And Setters For Mouse Sensitivity
	float GetMouseSensitivity() const;
	void SetMouseSensitivity(float _mouseSensitivity);

	//Getters And Setters For Camera Move Speed
	float GetCameraMoveSpeed() const;
	void SetCameraMoveSpeed(float _cameraMoveSpeed);
};

