#pragma once
#include "ObjectInstance.h"

//Moves Camera Left And Right By A Ampltiude of Units Across A Sine Wave
class TestCamera : public Component 
{
private:
	float speed;
	float amplitude;

	float time;

public:
	TestCamera(float _speed, float _amplitude);
	~TestCamera();

	void Update()override;
};