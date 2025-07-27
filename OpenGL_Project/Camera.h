#pragma once
#include "AssetLoader.h"

class Camera
{
protected:
	Camera();
	~Camera();

	glm::mat4 viewMatrix;

	glm::mat4 orthoProjectionMatrix;
	glm::mat4 perspectiveProjectionMatrix;
	glm::mat4 screenProjectionMatrix;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

	float nearPlane = 0.1f;
	float farPlane = 500.0f;

	float fieldOfView = 45.0f;
	float orthographicSize = 5.0f;


public:
	static Camera& Instance() { static Camera camera; return camera; }

	static void CalculateViewMatrix();
	static void CalculateProjectionMatrix();

	glm::mat4 GetProjectionMatrix(ProjectionType _type) const;
	glm::mat4 GetViewMatrix() const;
	
	glm::vec3 GetCameraPosition() const;
	void SetCameraPosition(glm::vec3 _position);

	glm::vec3 GetCameraLookDirection() const;
	void SetCameraLookDirection(glm::vec3 _direction);

	glm::vec3 GetCameraUpDirection() const;
	void SetCameraUpDirection(glm::vec3 _upDirection);

	float GetNearPlane() const;
	void SetNearPlane(float _nearPlane);

	float GetFarPlane() const;
	void SetFarPlane(float _farPlane);

	float GetFieldOfView() const;
	void SetFieldOfView(float _fieldOfView);

	float GetOrthographicSize() const;
	void SetOrthographicSize(float _orthographicSize);
};

