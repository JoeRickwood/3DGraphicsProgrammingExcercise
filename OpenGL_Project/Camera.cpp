#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::CalculateViewMatrix()
{
	Camera::Instance().viewMatrix = glm::lookAt(
		Camera::Instance().cameraPosition,
		Camera::Instance().cameraPosition + Camera::Instance().cameraLookDir,
		Camera::Instance().cameraUpDir
	);
}

void Camera::CalculateProjectionMatrix()
{
	float halfWidth = GraphicsLoader::Instance().windowSize.x / 2.0f;
	float halfheight = GraphicsLoader::Instance().windowSize.x / 2.0f;
	Camera::Instance().orthoProjectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfheight, halfheight, 0.1f, 100.f);

	float aspectRatio = GraphicsLoader::Instance().windowSize.x / GraphicsLoader::Instance().windowSize.y;

	Camera::Instance().perspectiveProjectionMatrix = glm::perspective(glm::radians(Camera::Instance().fieldOfView), aspectRatio, 0.1f, 100.f);

}

glm::mat4 Camera::GetProjectionMatrix(ProjectionType _type)
{
	switch (_type)
	{
	case Orthographic:
		return orthoProjectionMatrix;
		break;
	case Perspective:
		return perspectiveProjectionMatrix;
		break;
	}
}