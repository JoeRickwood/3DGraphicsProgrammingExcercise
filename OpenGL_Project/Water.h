#pragma once
#include "ObjectInstance.h"

class Water : public Component
{
private:
	float waveSpeed;
	float waveFrequency;
	float waveScale;

	float t;

public:
	Water(float _waveSpeed, float _waveFrequency, float _waveScale);
	~Water();

	void Update()override;
	void ShaderUpdate()override;
};

