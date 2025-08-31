#include "Firework.h"
#include "Time.h"
#include <iostream>

Firework::Firework(float _height)
{
	heightToActivate = _height;

	velocity = glm::vec3(((rand() % 100) - 50.0f), 100, ((rand() % 100) - 50.0f));
	gravity = 0;

	burst = false;
}

Firework::~Firework()
{
}

void Firework::Update()
{
	if (burst) 
	{
		return;
	}

	gravity -= Time::Instance().deltaTime;

	parent->SetPosition(parent->GetPosition() + (glm::vec3(0.0f, gravity, 0.0f) + velocity) * Time::Instance().deltaTime);

	if (parent->GetPosition().y >= heightToActivate && burst == false) 
	{
		burst = true;
		Burst();
	}
}

void Firework::Burst()
{
	parent->GetComponent<ParticleSystem>()->SetReset(false);

	ObjectInstance* burstObj = new ObjectInstance("FireworkBurst", parent->GetPosition(), parent->GetRotation(), parent->GetScale());
	auto particleRenderer = burstObj->AddComponent<ParticleSystem>("Unlit", "ComputeParticles", ProjectionType::Perspective);
	particleRenderer->SetShadowRendering(false);
	particleRenderer->SetReset(true);
	particleRenderer->SetColor(parent->GetComponent<ParticleSystem>()->GetColor());
	particleRenderer->SetReset(true);
	particleRenderer->SetRandom(10);
}
