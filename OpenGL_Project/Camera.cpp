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
	//Value Of 120 Comes From 16:9 Aspect Ratio
	//A Resolution Of 1920 horizontal / 16 = 120? 
	
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