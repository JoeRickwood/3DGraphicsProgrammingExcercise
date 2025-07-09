#include "Camera.h"

Camera::Camera()
{
	orthoProjectionMatrix = glm::mat4();
	perspectiveProjectionMatrix = glm::mat4();
	shadowProjectionPerspectiveMatrix = glm::mat4();

	viewMatrix = glm::mat4();
}

Camera::~Camera()
{
}

//Calculates The View Matrix Of Camera, This Takes Into Count The Camera Position, And Direction Its Facing
void Camera::CalculateViewMatrix()
{
	//Looks At The Camera View Direction
	Camera::Instance().viewMatrix = glm::lookAt(
		Camera::Instance().cameraPosition,
		Camera::Instance().cameraPosition + Camera::Instance().cameraLookDir,
		Camera::Instance().cameraUpDir
	);
}

//Calcualtes The Projection Matrix Used By Objects And Passed Into The Model Matrix
void Camera::CalculateProjectionMatrix()
{
	float aspect = AssetLoader::Instance().windowSize.x / AssetLoader::Instance().windowSize.y;

	float halfWidth = Instance().orthographicSize * aspect;
	float halfheight = Instance().orthographicSize;
	Instance().orthoProjectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfheight, halfheight, Instance().nearPlane, Instance().farPlane);

	float aspectRatio = AssetLoader::Instance().windowSize.x / AssetLoader::Instance().windowSize.y;
	Instance().perspectiveProjectionMatrix = glm::perspective(glm::radians(Instance().fieldOfView), aspectRatio, Instance().nearPlane, Instance().farPlane);
	Instance().shadowProjectionPerspectiveMatrix = glm::perspective(glm::radians(Instance().fieldOfView), aspectRatio, Instance().nearPlane, Instance().shadowFarPlane);
}

//Gets The Projection Matrix Of Either Orthographic Or Perspective, Returns Default Perspective
const glm::mat4 Camera::GetProjectionMatrix(ProjectionType _type)
{
	switch (_type)
	{
		case Orthographic:
			return orthoProjectionMatrix;
		case Perspective:
			return perspectiveProjectionMatrix;
		case ShadowPerspective:
			return shadowProjectionPerspectiveMatrix;
		default:
			return perspectiveProjectionMatrix;
	}
}

const glm::mat4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

const glm::vec3 Camera::GetCameraPosition()
{
	return cameraPosition;
}

void Camera::SetCameraPosition(glm::vec3 _position)
{
	cameraPosition = _position;
}

const glm::vec3 Camera::GetCameraLookDirection()
{
	return cameraLookDir;
}

void Camera::SetCameraLookDirection(glm::vec3 _direction)
{
	cameraLookDir = _direction;
}

const glm::vec3 Camera::GetCameraUpDirection()
{
	return cameraUpDir;
}

void Camera::SetCameraUpDirection(glm::vec3 _upDirection)
{
	cameraUpDir = _upDirection;
}


const float Camera::GetNearPlane()
{
	return nearPlane;
}

void Camera::SetNearPlane(float _nearPlane)
{
	nearPlane = _nearPlane;
}

const float Camera::GetFarPlane()
{
	return farPlane;
}

void Camera::SetFarPlane(float _farPlane)
{
	farPlane = _farPlane;
}

const float Camera::GetFieldOfView()
{
	return fieldOfView;
}

void Camera::SetFieldOfView(float _fieldOfView)
{
	fieldOfView = _fieldOfView;
}

const float Camera::GetShadowFarPlane()
{
	return shadowFarPlane;
}

void Camera::SetShadowFarPlane(float _shadowFarPlane)
{
	shadowFarPlane = _shadowFarPlane;
}

const float Camera::GetOrthographicSize()
{
	return orthographicSize;
}

void Camera::SetOrthographicSize(float _orthographicSize)
{
	orthographicSize = _orthographicSize;
}
