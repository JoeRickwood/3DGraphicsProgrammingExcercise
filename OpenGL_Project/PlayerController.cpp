#include "PlayerController.h"
#include "Time.h"
#include <iostream>
#include "Scene.h"
#include "MathFunctions.h"

PlayerController::PlayerController(float _mouseSensitivity, float _cameraMoveSpeed)
{
	mouseSensitivity = _mouseSensitivity;
	cameraMoveSpeed = _cameraMoveSpeed;

	velocity = glm::vec3(0.f, 0.f, 0.f);

	prevMousePos = glm::vec2(0.0f, 0.0f);
	mousePos = glm::vec2(0.0f, 0.0f);

	rotX = 0.f;
	rotY = 0.f;
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

	if (glfwGetMouseButton(AssetLoader::Instance().currentWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		//Camera Rotation
		prevMousePos = mousePos;

		double x;
		double y;
		glfwGetCursorPos(AssetLoader::Instance().currentWindow, &x, &y);
		mousePos.x = (float)x;
		mousePos.y = (float)y;

		glfwSetInputMode(AssetLoader::Instance().currentWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glm::vec2 moveDir = mousePos - prevMousePos;

		glm::vec3 rotation = parent->GetRotation();

		rotation.x += moveDir.y * mouseSensitivity;
		if (rotation.x < glm::radians(-89.f))
		{
			rotation.x = glm::radians(-89.f);
		}

		if (rotation.x > glm::radians(89.f))
		{
			rotation.x = glm::radians(89.f);
		}

		rotation.y -= moveDir.x * mouseSensitivity * 0.5f;

		glm::vec3 cameraLookDir = glm::vec3(
			cosf(rotation.x) * sin(rotation.y),
			-sinf(rotation.x),
			cosf(rotation.x) * cos(rotation.y)
		);

		parent->SetRotation(rotation);

		Camera::Instance().SetCameraLookDirection(cameraLookDir);

		//Movement
		glm::vec3 cameraRight = -glm::normalize(glm::cross(cameraLookDir, Camera::Instance().GetCameraUpDirection()));
		glm::vec3 cameraForward = -glm::normalize(cameraLookDir * glm::vec3(1.f, 0.f, 1.f));

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
			velocity = Lerp(velocity, targetVel, Time::Instance().deltaTime * 5.f);
		}
		else
		{
			velocity = Lerp(velocity, glm::vec3(0.f, 0.f, 0.f), Time::Instance().deltaTime * 5.f);
		}
	}
	else 
	{
		glfwSetInputMode(AssetLoader::Instance().currentWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		velocity = Lerp(velocity, glm::vec3(0.f, 0.f, 0.f), Time::Instance().deltaTime * 5.f);

		double x;
		double y;
		glfwGetCursorPos(AssetLoader::Instance().currentWindow, &x, &y);
		mousePos.x = (float)x;
		mousePos.y = (float)y;

		prevMousePos = mousePos;
	}

	parent->SetPosition(parent->GetPosition() + (-velocity * Time::Instance().deltaTime));

	Camera::Instance().SetCameraPosition(parent->GetPosition());
}

float PlayerController::GetMouseSensitivity() const
{
	return mouseSensitivity;
}

void PlayerController::SetMouseSensitivity(float _mouseSensitivity)
{
	mouseSensitivity = _mouseSensitivity;
}

float PlayerController::GetCameraMoveSpeed() const
{
	return cameraMoveSpeed;
}

void PlayerController::SetCameraMoveSpeed(float _cameraMoveSpeed)
{
	cameraMoveSpeed = _cameraMoveSpeed;
}
