#include "Button.h"


Button::Button()
{
	presslock = false;
}

Button::~Button()
{
}

void Button::Update()
{
	Bounds bounds = parent->GetComponent<Renderer>()->GetWorldBounds();

	GLFWwindow* window = glfwGetCurrentContext();

	int windowX, windowY;
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &windowX, &windowY);

	//glm::vec3 realpos = glm::unProject(window, glm::mat4(1.0f), glm::mat4(1.0f), glViewport(0, 0, windowX, windowY));

	xpos = (xpos / windowX) - 0.5f;
	ypos = (ypos / windowY) - 0.5f;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (bounds.Contains(xpos, ypos) && state == GLFW_PRESS && presslock == false)
	{
		Click();
		presslock = true;
	}

	if (state == GLFW_RELEASE)
	{
		presslock = false;
	} 
}

void Button::AddListener(void(*func)())
{
	listeners.push_back(func);
}

void Button::Click()
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]();
	}
}