#pragma once
#include "ObjectInstance.h"

class TestLerp : public Component 
{
private:
	int texID0;
	int texID1;
	float speed;

	float t;

public:
	TestLerp(float _speed, int _texID0, int _texID1);
	~TestLerp();

	void Update()override;
	void ShaderUpdate()override;
};