#pragma once
#include "GraphicsLoader.h"

class Camera
{
public:
	static Camera& Instance() { static Camera shaderLoader; return shaderLoader; }


	static void CalculateViewMatrix();
	static void CalculateProjectionMatrix();

	glm::mat4 GetProjectionMatrix(ProjectionType _type);

	//Camera Stuff
	glm::mat4 viewMatrix;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 25.0f, 10.0f);
	glm::vec3 cameraLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

	float nearPlane = 0.1f;
	float farPlane = 100.f;
	float fieldOfView = 45.f;

private:
	Camera();
	~Camera();

	glm::mat4 orthoProjectionMatrix;
	glm::mat4 perspectiveProjectionMatrix;
};

