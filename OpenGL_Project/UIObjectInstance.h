#pragma once
#include "ObjectInstance.h"

class UIObjectInstance : public ObjectInstance
{
protected:
	ScreenAlignmentX xScreenAlignment;
	ScreenAlignmentY yScreenAlignment;

public:
	UIObjectInstance(std::string _name, glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _rotation = glm::vec3(0.0f), glm::vec3 _scale = glm::vec3(1.f));
	~UIObjectInstance();

	glm::vec3 GetPosition() const override;

	void SetHorizontalScreenAlignment(ScreenAlignmentX _screenAlignmentX);
	ScreenAlignmentX GetHorizontalScreenAlignment() const;

	void SetVerticalScreenAlignment(ScreenAlignmentY _screenAlignmentY);
	ScreenAlignmentY GetVerticalScreenAlignment() const;

	void SetScreenAlignment(ScreenAlignmentX _screenAlignmentX, ScreenAlignmentY _screenAlignmentY);
};

