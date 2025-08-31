#pragma once
#include "ParticleSystem.h"

class Firework : public Component
{
protected:
	float heightToActivate;
	float gravity;

	glm::vec3 velocity;


	bool burst = false;

public:
	Firework(float _height);
	~Firework();

	void Update()override;
	void Burst();
};

