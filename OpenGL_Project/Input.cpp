#include "Input.h"

bool Input::GetKey(int key)
{
	return keystates[key];
}

void Input::Init()
{
	keystates = new bool[GLFW_KEY_LAST];

	GLFWwindow* window = glfwGetCurrentContext();

	glfwSetKeyCallback(window, (GLFWkeyfun)KeyCallback);
}

Input::Input()
{
	keystates = nullptr;
}

Input::~Input()
{

}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) 
	{
		Input::Instance().keystates[key] = true;
	}

	if (action == GLFW_RELEASE) 
	{
		Input::Instance().keystates[key] = false;
	}
}
