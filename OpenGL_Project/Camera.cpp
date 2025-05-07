#include "Camera.h"

Camera::Camera()
{
	orthoProjectionMatrix = glm::mat4();
	perspectiveProjectionMatrix = glm::mat4();
	viewMatrix = glm::mat4();
}

Camera::~Camera()
{
}

//Calculates The View Matrix Of Camera, This Takes Into Count The Camera Position, And Direction Its Facing
void Camera::CalculateViewMatrix()
{
	Camera::Instance().viewMatrix = glm::lookAt(
		Camera::Instance().cameraPosition,
		Camera::Instance().cameraPosition + Camera::Instance().cameraLookDir,
		Camera::Instance().cameraUpDir
	);
}

//Calcualtes The Projection Matrix Used By Objects And Passed Into The Model Matrix
void Camera::CalculateProjectionMatrix()
{
	float halfWidth = GraphicsLoader::Instance().windowSize.x / 2.0f;
	float halfheight = GraphicsLoader::Instance().windowSize.y / 2.0f;

	Camera::Instance().orthoProjectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfheight, halfheight, Camera::Instance().nearPlane, Camera::Instance().farPlane);

	float aspectRatio = GraphicsLoader::Instance().windowSize.x / GraphicsLoader::Instance().windowSize.y;

	Camera::Instance().perspectiveProjectionMatrix = glm::perspective(glm::radians(Camera::Instance().fieldOfView), aspectRatio, Camera::Instance().nearPlane, Camera::Instance().farPlane);
}

//Gets The Projection Matrix Of Either Orthographic Or Perspective, Returns Default Perspective
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
	default:
		return perspectiveProjectionMatrix;
	}
}