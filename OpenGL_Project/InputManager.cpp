#include "InputManager.h"
#include <iostream>

void InputManager::Init()
{
	glfwSetKeyCallback(glfwGetCurrentContext(), (GLFWkeyfun)KeyCallback);

	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		keystates[i] = false;
	}
}

bool InputManager::GetKey(int _keyCheck)
{
	return keystates[_keyCheck];
}


InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::KeyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{

	if (_action == GLFW_PRESS)
	{
		InputManager::Instance().keystates[_key] = true;
	}

	if (_action == GLFW_RELEASE)
	{
		InputManager::Instance().keystates[_key] = false;
	}
}
