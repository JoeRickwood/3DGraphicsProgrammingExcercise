#include "PlayerController.h"
#include "Time.h"
#include <iostream>

PlayerController::PlayerController(float _moveSpeed, float _height, float _mouseSensitivity)
{
	moveSpeed = _moveSpeed;
	height = _height;

	velocity = glm::vec3();
	gravity = 0.f;

	jumpResetTimer = 0.f;

	prevMousePos = glm::vec2(0.0f, 0.0f);
	mousePos = glm::vec2(0.0f, 0.0f);

	mouseSensitivity = _mouseSensitivity;
}

PlayerController::~PlayerController()
{
	
}

void PlayerController::Update()
{
	//Camera Rotation
	prevMousePos = mousePos;

	glfwSetInputMode(GraphicsLoader::Instance().currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double x;
	double y;
	glfwGetCursorPos(GraphicsLoader::Instance().currentWindow, &x, &y);
	mousePos.x = x / GraphicsLoader::Instance().windowSize.x;
	mousePos.y = y / GraphicsLoader::Instance().windowSize.y;

	glm::vec2 moveDir = mousePos - prevMousePos;

	parent->rotation.x += moveDir.y * mouseSensitivity;
	if (parent->rotation.x < -89.f) 
	{
		parent->rotation.x = -89.f;
	}

	if (parent->rotation.x > 89.f) 
	{
		parent->rotation.x = 89.f;
	}

	parent->rotation.y -= moveDir.x * mouseSensitivity;

	Camera::Instance().cameraLookDir.x = cosf(parent->rotation.x) * sin(parent->rotation.y);
	Camera::Instance().cameraLookDir.y = -sinf(parent->rotation.x);
	Camera::Instance().cameraLookDir.z = cosf(parent->rotation.x) * cos(parent->rotation.y);


	//Movement
	gravity -= Time::Instance().deltaTime * 9.81f;
	jumpResetTimer -= Time::Instance().deltaTime;

	glm::vec3 cameraRight = glm::cross(Camera::Instance().cameraLookDir, Camera::Instance().cameraUpDir);

	velocity =
		(cameraRight * ((glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_D) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_A) == GLFW_PRESS ? -1.f : 0.f)) +
		Camera::Instance().cameraLookDir * ((glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_W) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_S) == GLFW_PRESS ? -1.f : 0.f))) * moveSpeed;

	if (jumpResetTimer <= 0.f)
	{
		if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Jump();
		}
	}



	velocity.y = gravity;

	Camera::Instance().cameraPosition = parent->position + glm::vec3(0.f, height, 0.f);

	float prevY = parent->position.y;
	parent->position.y += velocity.y * Time::Instance().deltaTime;
	if (parent->position.y < 0.f) 
	{
		velocity.y = 0.f;
		if (jumpResetTimer <= 0.f) 
		{
			gravity = 0.f;
		}
		parent->position.y = prevY;
	}

	parent->position.x += velocity.x * Time::Instance().deltaTime;
	parent->position.z += velocity.z * Time::Instance().deltaTime;
}

void PlayerController::Jump()
{
	gravity = jumpHeight;
	jumpResetTimer = 0.2f;
}
