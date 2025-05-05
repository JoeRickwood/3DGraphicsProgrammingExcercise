#include "CameraController.h"
#include "Time.h"

CameraController::CameraController(float _moveSpeed, float _distance)
{
	moveSpeed = _moveSpeed;
	distance = _distance;

	targetPosition = glm::vec3(0.f, 0.f, 0.f);
	position = glm::vec3(0.f, 0.f, 0.f);

	t = 0.f;

	height = 20.f;
}

CameraController::~CameraController()
{

}

void CameraController::Init()
{
}

void CameraController::Update()
{
	if (enabled == false) 
	{
		return;
	}

	t += Time::Instance().deltaTime * moveSpeed;

	height += Time::Instance().deltaTime * (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_W) == GLFW_PRESS ? 1.f : 0.f) * 10.f;
	height += Time::Instance().deltaTime * (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_S) == GLFW_PRESS ? -1.f : 0.f) * 10.f;

	float x = sinf(t) * distance;
	float z = cosf(t) * distance;

	position = glm::vec3(x, height, z);

	Camera::Instance().cameraPosition = position;
	Camera::Instance().cameraLookDir = targetPosition - position;
}
