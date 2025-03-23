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
		(Input::Instance().GetKey(GLFW_KEY_A) == true ? -1.f : 0.f) + (Input::Instance().GetKey(GLFW_KEY_D) == true ? 1.f : 0.f),
		(Input::Instance().GetKey(GLFW_KEY_W) == true ? 1.f : 0.f) + (Input::Instance().GetKey(GLFW_KEY_S) == true ? -1.f : 0.f),
		(Input::Instance().GetKey(GLFW_KEY_LEFT_CONTROL) == true ? -1.f : 0.f) + (Input::Instance().GetKey(GLFW_KEY_SPACE) == true ? 1.f : 0.f)
	);

	glm::vec3 right = glm::cross(Camera::Instance().cameraLookDir, Camera::Instance().cameraUpDir);

	glm::vec3 velocity = (Camera::Instance().cameraLookDir * input.y) + (right * input.x) + (glm::vec3(0.f, 1.f, 0.f) * input.z);

	Camera::Instance().cameraPosition += velocity * Time::Instance().deltaTime * moveSpeed;
}
