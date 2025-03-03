#include "Components.h"

InputMove::InputMove()
{
	inputX = 0.f;
	inputY = 0.f;
	rotationInput;
}

InputMove::~InputMove()
{
}

void InputMove::Update()
{
	inputX = (InputManager::Instance().GetKey(GLFW_KEY_A) == true ? -1.f : 0.f) + (InputManager::Instance().GetKey(GLFW_KEY_D) == true ? 1.f : 0.f);
	inputY = (InputManager::Instance().GetKey(GLFW_KEY_S) == true ? -1.f : 0.f) + (InputManager::Instance().GetKey(GLFW_KEY_W) == true ? 1.f : 0.f);
	rotationInput = (InputManager::Instance().GetKey(GLFW_KEY_R) == true ? -1.f : 0.f);
	scaleInput = (InputManager::Instance().GetKey(GLFW_KEY_MINUS) == true ? -1.f : 0.f) + (InputManager::Instance().GetKey(GLFW_KEY_EQUAL) == true ? 1.f : 0.f);

	parent->position += glm::vec3(0.2f * inputX, 0.2f * inputY, 0.f) * (1.f / 60.f);
	parent->rotation += rotationInput * 1.f;
	parent->scale += glm::vec3(1.f, 1.f, 1.f) * scaleInput * 0.01f;
}
