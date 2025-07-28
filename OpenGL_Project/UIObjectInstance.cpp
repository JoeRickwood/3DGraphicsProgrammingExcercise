#include "UIObjectInstance.h"

UIObjectInstance::UIObjectInstance(std::string _name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale) : ObjectInstance(_name, _position, _rotation, _scale)
{
	xScreenAlignment = LEFT;
	yScreenAlignment = BOTTOM;
}

UIObjectInstance::~UIObjectInstance()
{
}

glm::vec3 UIObjectInstance::GetPosition() const
{
	float posX = 0;
	float posY = 0;

	glm::vec2 windowSize = AssetLoader::Instance().GetWindowSize();

	switch (xScreenAlignment)
	{
	case LEFT:
		posX = 0;
		break;
	case MIDDLE:
		posX = windowSize.x / 2.0f;
		break;
	case Right:
		posX = windowSize.x;
		break;
	}

	switch (yScreenAlignment)
	{
	case BOTTOM:
		posY = 0;
		break;
	case CENTER:
		posY = windowSize.y / 2.0f;
		break;
	case TOP:
		posY = windowSize.y;
		break;
	}

	return glm::vec3(posX + position.x, posY + position.y, position.z);
}

void UIObjectInstance::SetHorizontalScreenAlignment(ScreenAlignmentX _screenAlignmentX)
{
	xScreenAlignment = _screenAlignmentX;
}

ScreenAlignmentX UIObjectInstance::GetHorizontalScreenAlignment() const
{
	return xScreenAlignment;
}

void UIObjectInstance::SetVerticalScreenAlignment(ScreenAlignmentY _screenAlignmentY)
{
	yScreenAlignment = _screenAlignmentY;
}

ScreenAlignmentY UIObjectInstance::GetVerticalScreenAlignment() const
{
	return yScreenAlignment;
}

void UIObjectInstance::SetScreenAlignment(ScreenAlignmentX _screenAlignmentX, ScreenAlignmentY _screenAlignmentY)
{
	xScreenAlignment = _screenAlignmentX;
	yScreenAlignment = _screenAlignmentY;
}
