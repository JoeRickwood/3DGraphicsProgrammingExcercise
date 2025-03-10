#pragma once
#include "ObjectInstance.h"

class Tests : public Component 
{
public:
	Tests(float _speed, float _offset, float _rotationSpeed);
	~Tests();

	void Update()override;
	void ShaderUpdate()override;

private:
	float time;

	float speed;
	float offset;
	float rotationSpeed;


	float x = 0.f;
	float y = 0.f;
	float rot = 0.f;
};
