#include "Camera.h"

Camera::Camera()
{
	orthoProjectionMatrix = glm::mat4();
	perspectiveProjectionMatrix = glm::mat4();
	screenProjectionMatrix = glm::mat4();

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
	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();
	float aspect = windowSize.x / windowSize.y;

	float halfWidth = Instance().orthographicSize * aspect;
	float halfheight = Instance().orthographicSize;
	Instance().orthoProjectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfheight, halfheight, Instance().nearPlane, Instance().farPlane);
	Instance().screenProjectionMatrix = glm::ortho(0.0f, windowSize.x, 0.0f, windowSize.y);

	float aspectRatio = windowSize.x / windowSize.y;
	Instance().perspectiveProjectionMatrix = glm::perspective(glm::radians(Instance().fieldOfView), aspectRatio, Instance().nearPlane, Instance().farPlane);
}

//Gets The Projection Matrix Of Either Orthographic Or Perspective, Returns Default Perspective
glm::mat4 Camera::GetProjectionMatrix(ProjectionType _type) const
{
	switch (_type)
	{
		case ORTHOGRAPHIC:
			return orthoProjectionMatrix;
		case PERSPECTIVE:
			return perspectiveProjectionMatrix;
		case SCREEN_ORTHOGRAPHIC:
			return screenProjectionMatrix;
		default:
			return perspectiveProjectionMatrix;
	}
}

glm::mat4 Camera::GetViewMatrix() const
{
	return viewMatrix;
}

glm::vec3 Camera::GetCameraPosition() const
{
	return cameraPosition;
}

void Camera::SetCameraPosition(glm::vec3 _position)
{
	cameraPosition = _position;
}

glm::vec3 Camera::GetCameraLookDirection() const
{
	return cameraLookDir;
}

void Camera::SetCameraLookDirection(glm::vec3 _direction)
{
	cameraLookDir = _direction;
}

glm::vec3 Camera::GetCameraUpDirection() const
{
	return cameraUpDir;
}

void Camera::SetCameraUpDirection(glm::vec3 _upDirection)
{
	cameraUpDir = _upDirection;
}


float Camera::GetNearPlane() const
{
	return nearPlane;
}

void Camera::SetNearPlane(float _nearPlane)
{
	nearPlane = _nearPlane;
}

float Camera::GetFarPlane() const
{
	return farPlane;
}

void Camera::SetFarPlane(float _farPlane)
{
	farPlane = _farPlane;
}

float Camera::GetFieldOfView() const
{
	return fieldOfView;
}

void Camera::SetFieldOfView(float _fieldOfView)
{
	fieldOfView = _fieldOfView;
}


float Camera::GetOrthographicSize() const
{
	return orthographicSize;
}

void Camera::SetOrthographicSize(float _orthographicSize)
{
	orthographicSize = _orthographicSize;
}
