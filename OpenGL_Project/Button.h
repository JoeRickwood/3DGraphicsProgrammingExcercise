#pragma once
#include "ObjectInstance.h"
#include <functional>

class Button : public Component
{
protected:
	bool mouseOver;
	bool pressed;

	glm::vec4 color;

	std::vector<std::function<void()>> listeners;

	//Presslock To Prevent Pressing Each Frame
	bool pressLock = false;

	glm::vec3 initialScale;


	//Intersection Check With Input Position, Used To See if Mouse Is Over 
	const bool Intersects(glm::vec3 _position) const;

public:
	Button();
	~Button();

	void Init()override;
	void Update()override;
	void AddListener(std::function<void()> _func); //Adds New Listener To Button
	void Click(); //Calls All Active Listeners On The Buttons When Called

	//Getters And Setters For Color Values
	glm::vec4 GetColor() const;
	void SetColor(glm::vec4 _color);

	//Returns If Buttons Is Currently Held Down Or Not
	bool GetPressedState() const;
};

