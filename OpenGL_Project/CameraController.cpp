#include "CameraController.h"

CameraController::CameraController(float _moveSpeed)
{
	moveSpeed = _moveSpeed;

	input = glm::vec3(0.f, 0.f, 0.f);
}

CameraController::~CameraController()
{

}

void CameraController::Init()
{
}

void CameraController::Update()
{
	input = glm::vec3(
		(glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_A) == GLFW_PRESS ? -1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_D) == GLFW_PRESS ? 1.f : 0.f),
		(glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? -1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_SPACE) == GLFW_PRESS ? 1.f : 0.f),
		(glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_W) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_S) == GLFW_PRESS ? -1.f : 0.f)
		
	);

	glm::vec3 right = glm::cross(Camera::Instance().cameraLookDir, Camera::Instance().cameraUpDir);

	glm::vec3 velocity = (Camera::Instance().cameraLookDir * input.y) + (right * input.x) + (glm::vec3(0.f, 1.f, 0.f) * input.z);

	Camera::Instance().cameraPosition += velocity * Time::Instance().deltaTime * moveSpeed;
}
