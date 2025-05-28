#include "PlayerController.h"
#include "Time.h"
#include <iostream>
#include "Scene.h"

PlayerController::PlayerController(float _mouseSensitivity = 3.f, float _cameraMoveSpeed = 10.f)
{
	mouseSensitivity = _mouseSensitivity;
	cameraMoveSpeed = _cameraMoveSpeed;

	velocity = glm::vec3(0.f, 0.f, 0.f);

	prevMousePos = glm::vec2(0.0f, 0.0f);
	mousePos = glm::vec2(0.0f, 0.0f);
}

PlayerController::~PlayerController()
{
	
}

void PlayerController::Update()
{

	if (enabled == false)
	{
		return;
	}


	//Camera Rotation
	prevMousePos = mousePos;

	glfwSetInputMode(AssetLoader::Instance().currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	double x;
	double y;
	glfwGetCursorPos(AssetLoader::Instance().currentWindow, &x, &y);
	mousePos.x = (float)x / AssetLoader::Instance().windowSize.x;
	mousePos.y = (float)y / AssetLoader::Instance().windowSize.y;

	glm::vec2 moveDir = mousePos - prevMousePos;
	if (moveDir.x > 1.f) 
	{
		moveDir.x = 1.f;
	}
	else if (moveDir.x < -1.f) 
	{
		moveDir.x = -1.f;
	}

	if (moveDir.y > 1.f)
	{
		moveDir.y = 1.f;
	}
	else if (moveDir.y < -1.f)
	{
		moveDir.y = -1.f;
	}

	parent->rotation.x += moveDir.y * mouseSensitivity;
	if (parent->rotation.x < glm::radians(-89.f))
	{
		parent->rotation.x = glm::radians(-89.f);
	}

	if (parent->rotation.x > glm::radians(89.f))
	{
		parent->rotation.x = glm::radians(89.f);
	}

	parent->rotation.y -= moveDir.x * mouseSensitivity;

	Camera::Instance().cameraLookDir.x = cosf(parent->rotation.x) * sin(parent->rotation.y);
	Camera::Instance().cameraLookDir.y = -sinf(parent->rotation.x);
	Camera::Instance().cameraLookDir.z = cosf(parent->rotation.x) * cos(parent->rotation.y);

	//Movement
	glm::vec3 cameraRight = -glm::normalize(glm::cross(Camera::Instance().cameraLookDir, Camera::Instance().cameraUpDir));
	glm::vec3 cameraForward = -glm::normalize(Camera::Instance().cameraLookDir * glm::vec3(1.f, 0.f, 1.f));

	//Setting Velocity With Input Multiplied By camera Vectors And Speeds
	glm::vec3 targetVel =
		(cameraRight * ((glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_D) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_A) == GLFW_PRESS ? -1.f : 0.f)) + //Left Right
		cameraForward * ((glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_W) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_S) == GLFW_PRESS ? -1.f : 0.f)) + //Forward Backward
			glm::vec3(0.f, 1.f, 0.f) * ((glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_E) == GLFW_PRESS ? -1.f : 0.f) + (glfwGetKey(AssetLoader::Instance().currentWindow, GLFW_KEY_Q) == GLFW_PRESS ? 1.f : 0.f))); //Up Down

	if (targetVel != glm::vec3(0.f, 0.f, 0.f)) 
	{
		//Normalize To Remove Length Improportions
		targetVel = glm::normalize(targetVel) * cameraMoveSpeed;

		//Interpolate The velocity For "Smooth" Effect
		velocity = lerp(velocity, targetVel, Time::Instance().deltaTime * 5.f);
	}
	else 
	{
		velocity = lerp(velocity, glm::vec3(0.f, 0.f, 0.f), Time::Instance().deltaTime * 5.f);
	}

	parent->position += -velocity * Time::Instance().deltaTime;

	Camera::Instance().cameraPosition = parent->position;
}
