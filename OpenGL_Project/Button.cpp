#include "Button.h"
#include "Renderer.h"
#include <iostream>

const bool Button::Intersects(glm::vec3 _position) const
{

	//Calculate The Bounds Of The Button
	float left = parent->position.x - (parent->scale.x / 2.f);
	float right = parent->position.x + (parent->scale.x / 2.f);

	float up = parent->position.y + (parent->scale.y / 2.f);
	float down = parent->position.y - (parent->scale.y / 2.f);

	//Compare To Mouse Pos
	if (_position.x > left && _position.x < right && _position.y < up && _position.y > down) 
	{
		return true;
	}

	return false;
}

//Debugs The Current Position Of the Mouse In Screen Coordinates And Then Remapped To Be In The Same Space As The Button
void Button::DebugPos()
{
	double x;
	double y;

	int windowSizeX;
	int windowSizeY;

	GLFWwindow* window = glfwGetCurrentContext();

	glfwGetCursorPos(window, &x, &y);
	glfwGetWindowSize(window, &windowSizeX, &windowSizeY);

	std::cout << "Mouse Coordinates On Screen :" << x << "," << y << std::endl;
	std::cout << "Mouse Coordinates Remapped :" << (x / windowSizeX) * 2.f - 1.f << "," << -((y / windowSizeY) * 2.f - 1.f) << std::endl << std::endl;
}

Button::Button(int _texID0, int _texID1)
{
	debugToggle = false;
	debugToggleLock = false;
	mousePos = glm::vec2(0.f, 0.f);

	mouseOver = false;

	textureID0 = _texID0;
	textureID1 = _texID1;
}

Button::~Button()
{
}

void Button::Update()
{
	//Create A Toggle-Action State Of The '3' 
	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_3) == GLFW_PRESS && debugToggleLock == false)
	{
		debugToggle = !debugToggle;
		debugToggleLock = true;
		DebugPos();
	}

	if (glfwGetKey(GraphicsLoader::Instance().currentWindow, GLFW_KEY_3) == GLFW_RELEASE && debugToggleLock)
	{
		debugToggleLock = false;
	}



	double x;
	double y;

	int windowSizeX;
	int windowSizeY;

	GLFWwindow* window = glfwGetCurrentContext();
	glfwGetCursorPos(window, &x, &y);

	glfwGetWindowSize(window, &windowSizeX, &windowSizeY);

	//Remap Mouse Pos To Be In Correct Spacing
	mousePos.x = ((float)x / (float)windowSizeX) * 2.f - 1.f;
	mousePos.y = ((float)y / (float)windowSizeY) * 2.f - 1.f;

	//std::cout << mousePos.x << "," << mousePos.y << std::endl;

	//Check If The Mouse Posituion Intersects The Buttons' Global Bounds
	mouseOver = Intersects(glm::vec3(mousePos.x, -mousePos.y, 0.f));

	//If It Does Intersects, Change The TextureID to the alternate texture
	if (mouseOver) 
	{
		parent->GetComponent<Renderer>()->textureID = textureID1;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pressLock == false)
		{
			Click();
			pressLock = true;
		}
	}
	else //If It Does Not Intersect, Change The TextureID to the base Texture
	{
		parent->GetComponent<Renderer>()->textureID = textureID0;
	}

	//Reset Press Lock If The Mouse Button Is Released
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		pressLock = false;
	}
}

void Button::AddListener(std::function<void()> _func)
{
	listeners.push_back(_func);
}

void Button::Click()
{
	for (int i = 0; i < listeners.size(); i++)
	{
		listeners[i]();
	}
}
