#include "Button.h"
#include "Renderer.h"
#include <iostream>

const bool Button::Intersects(glm::vec3 _position) const
{

	//Calculate The Bounds Of The Button
	glm::vec3 position = parent->GetPosition();
	glm::vec3 scale = parent->GetScale();

	float left = position.x - (scale.x / 2.f);
	float right = position.x + (scale.x / 2.f);

	float up = position.y + (scale.y / 2.f);
	float down = position.y - (scale.y / 2.f);

	//Compare To Mouse Pos
	if (_position.x > left && _position.x < right && _position.y < up && _position.y > down) 
	{
		return true;
	}

	return false;
}


Button::Button()
{
	mousePos = glm::vec2(0.f, 0.f);

	mouseOver = false;
}

Button::~Button()
{
}

void Button::Update()
{
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
		//parent->GetComponent<Renderer>()->textureID = textureID1;

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pressLock == false)
		{
			Click();
			pressLock = true;
		}
	}
	else //If It Does Not Intersect, Change The TextureID to the base Texture
	{
		//parent->GetComponent<Renderer>()->textureID = textureID0;
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

glm::vec4 Button::GetColor()
{
	return color;
}

void Button::SetColor(glm::vec4 _color)
{
	color = _color;
}
