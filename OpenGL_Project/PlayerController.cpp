#include "PlayerController.h"
#include "Time.h"
#include <iostream>
#include "PhysicsObject.h"
#include "Scene.h"

PlayerController::PlayerController(float _moveSpeed, float _height, float _mouseSensitivity)
{
	moveSpeed = _moveSpeed;
	height = _height;

	jumpResetTimer = 0.f;

	prevMousePos = glm::vec2(0.0f, 0.0f);
	mousePos = glm::vec2(0.0f, 0.0f);

	mouseSensitivity = _mouseSensitivity;

	grounded = false;
}

PlayerController::~PlayerController()
{
	
}

void PlayerController::Update()
{
	grounded = Scene::Current().OverlapPoint(parent->position - glm::vec3(0.f, (height / 2.f) + 0.1f, 0.f));


	auto physicsObject = parent->GetComponent<PhysicsObject>();

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
	physicsObject->velocity.y -= Time::Instance().deltaTime * 9.81f;
	jumpResetTimer -= Time::Instance().deltaTime;

	glm::vec3 cameraRight = glm::cross(Camera::Instance().cameraLookDir, Camera::Instance().cameraUpDir);

	glm::vec3 tmpVelocity =
		(cameraRight * ((glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_D) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_A) == GLFW_PRESS ? -1.f : 0.f)) +
		Camera::Instance().cameraLookDir * ((glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_W) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_S) == GLFW_PRESS ? -1.f : 0.f))) * moveSpeed;

	if (jumpResetTimer <= 0.f && grounded)
	{
		if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Jump();
		}
	}

	tmpVelocity.y = physicsObject->velocity.y;
	physicsObject->velocity = tmpVelocity;

	Camera::Instance().cameraPosition = parent->position + glm::vec3(0.f, height, 0.f);
}

void PlayerController::Jump()
{
	auto physicsObject = parent->GetComponent<PhysicsObject>();

	physicsObject->velocity.y = jumpHeight;
	jumpResetTimer = 0.2f;
}
