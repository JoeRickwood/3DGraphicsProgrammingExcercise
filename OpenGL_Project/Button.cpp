#include "Button.h"
#include "Renderer.h"
#include <iostream>
#include "MathExtensions.h"
#include "Time.h"

const bool Button::Intersects(glm::vec3 _position) const
{
	//Calculate The Bounds Of The Button
	glm::vec3 position = parent->GetPosition();
	glm::vec3 scale = parent->GetScale() * 2.0f;

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
	mouseOver = false;

	color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pressed = false;
	initialScale = glm::vec3(0.0f, 0.0f, 0.0f);
}

Button::~Button()
{
}

void Button::Init()
{
	initialScale = parent->GetLocalScale();
}

void Button::Update()
{
	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();

	double x;
	double y;
	GLFWwindow* window = glfwGetCurrentContext();
	glfwGetCursorPos(window, &x, &y);

	//Check If The Mouse Posituion Intersects The Buttons' Global Bounds
	mouseOver = Intersects(glm::vec3(x, AssetLoader::Instance().GetWindowSize().y - y - 24, 0.f));

	//If It Does Intersects, Change The TextureID to the alternate texture
	if (mouseOver) 
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && pressLock == false)
		{
			Click();
			pressLock = true;
		}
	}

	//Reset Press Lock If The Mouse Button Is Released
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		pressLock = false;
		pressed = false;
	}

	color = Lerp(color, pressed ? glm::vec4(0.5f, 0.5f, 0.5f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), Time::Instance().deltaTime * 50.0f);
	parent->SetScale(Lerp(parent->GetLocalScale(), mouseOver ? initialScale * 1.1f : initialScale, Time::Instance().deltaTime * 50.0f));

	parent->GetComponent<Renderer>()->SetColor(color);
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

	pressed = true;
}

glm::vec4 Button::GetColor() const
{
	return color;
}

void Button::SetColor(glm::vec4 _color) 
{
	color = _color;
}

bool Button::GetPressedState() const
{
	return pressed;
}
