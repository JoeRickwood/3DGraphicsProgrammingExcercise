#include "PlayerController.h"
#include "Time.h"
#include <iostream>
#include "Scene.h"

PlayerController::PlayerController(float _objectMoveSpeed = 3.f, float _cameraMoveSpeed = 10.f, float _distance = 100.f, int _altTex = 0, int _altTex1 = 0)
{
	objectMoveSpeed = _objectMoveSpeed;
	cameraMoveSpeed = _cameraMoveSpeed;

	velocity = glm::vec3(0.f, 0.f, 0.f);

	distance = _distance;

	targetPosition = glm::vec3(0.f, 0.f, 0.f);
	position = glm::vec3(0.f, 0.f, 0.f);

	t = 0.f;

	height = 20.f;

	tex0 = _altTex;
	tex1 = _altTex1;

	texToggle = false;
	mouseCursorVisibility = false;
	mouseCursorVisibilityToggleLock = false;

	cameraToggle = false;
	cameraToggleLock = false;

	wireframeMode = false;
	wireframeToggleLock = false;
}

PlayerController::~PlayerController()
{
	
}

void PlayerController::HandleStates()
{
	//Camera Toggle
	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_TAB) == GLFW_PRESS && cameraToggleLock == false)
	{
		cameraToggle = !cameraToggle;
		cameraToggleLock = true;
	}

	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_TAB) == GLFW_RELEASE && cameraToggleLock)
	{
		cameraToggleLock = false;
	}



	//Wireframe Toggle, Uses '2' Key
	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_2) == GLFW_PRESS && wireframeToggleLock == false)
	{
		wireframeMode = !wireframeMode;
		wireframeToggleLock = true;

		if (wireframeMode) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_2) == GLFW_RELEASE && wireframeToggleLock)
	{
		wireframeToggleLock = false;
	}


	//Mouse Visibility, Uses '1' Key
	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_1) == GLFW_PRESS && mouseCursorVisibilityToggleLock == false)
	{

		mouseCursorVisibility = !mouseCursorVisibility;
		mouseCursorVisibilityToggleLock = true;
		glfwSetInputMode(GraphicsLoader::Instance().currentWindow, GLFW_CURSOR, mouseCursorVisibility ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_1) == GLFW_RELEASE && mouseCursorVisibilityToggleLock)
	{
		mouseCursorVisibilityToggleLock = false;
	}
}

void PlayerController::Update()
{

	if (enabled == false)
	{
		return;
	}

	HandleStates();

	//Camera Rotation
	// 
	//Disabling Mouse Cursor

	//Movement
	glm::vec3 cameraRight = glm::normalize(glm::cross(Camera::Instance().cameraLookDir, Camera::Instance().cameraUpDir));
	glm::vec3 cameraForward = glm::normalize(Camera::Instance().cameraLookDir * glm::vec3(1.f, 0.f, 1.f));

	//Setting Velocity With Input Multiplied By camera Vectors And Speeds
	glm::vec3 targetVel =
		(cameraRight * ((glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_D) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_A) == GLFW_PRESS ? -1.f : 0.f)) + //Left Right
		cameraForward * ((glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_W) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_S) == GLFW_PRESS ? -1.f : 0.f)) + //Forward Backward
			glm::vec3(0.f, 1.f, 0.f) * ((glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_E) == GLFW_PRESS ? 1.f : 0.f) + (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_Q) == GLFW_PRESS ? -1.f : 0.f))); //Up Down

	if (targetVel != glm::vec3(0.f, 0.f, 0.f)) 
	{
		//Normalize To Remove Length Improportions
		targetVel = glm::normalize(targetVel) * objectMoveSpeed;

		//Interpolate The velocity For "Smooth" Effect
		velocity = lerp(velocity, targetVel, Time::Instance().deltaTime * 5.f);
	}
	else 
	{
		velocity = lerp(velocity, glm::vec3(0.f, 0.f, 0.f), Time::Instance().deltaTime * 5.f);
	}




	parent->position += velocity * Time::Instance().deltaTime;


	//Camera Controlling
	t += Time::Instance().deltaTime * cameraMoveSpeed * cameraToggle;

	height += Time::Instance().deltaTime * (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_SPACE) == GLFW_PRESS ? 1.f : 0.f) * 10.f;
	height += Time::Instance().deltaTime * (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? -1.f : 0.f) * 10.f;

	float x = sinf(t) * distance;
	float z = cosf(t) * distance;

	position = glm::vec3(x, height, z);

	Camera::Instance().cameraPosition = position;
	Camera::Instance().cameraLookDir = targetPosition - position;
}

void PlayerController::ToggleTexture()
{
	texToggle = !texToggle;

	parent->GetComponent<Renderer>()->textureID = texToggle ? tex0 : tex1;


}
