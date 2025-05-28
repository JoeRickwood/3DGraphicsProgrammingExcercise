#pragma once
#include "ObjectInstance.h"

class PointLight : public Component
{
public:
	glm::vec3 color;
	float specularStrength;

	float attenuationConstant;
	float attenuationLinear;
	float attenuationExponent;

	PointLight(glm::vec3 _color = glm::vec3(0.0f, 0.0f, 0.0f), float _specularStrength = 0.0f,
		float _attenuationConstant = 1.0f, float _attenuationLinear = 0.045f, float _attenuationExponent = 0.0075f);

	~PointLight();

	void Init()override;
};

class DirectionalLight : public Component
{
public:
	glm::vec3 direction;
	glm::vec3 color;
	float specularStrength;

	DirectionalLight(glm::vec3 _direction = glm::vec3(0.3f, -0.6f, 0.3f), glm::vec3 _color = glm::vec3(1.f, 0.95f, 0.3f), float _specularStrength = 1.f);

	~DirectionalLight();

	void Init()override;
};

class SpotLight : public Component
{
public:
	glm::vec3 direction;
	glm::vec3 color;

	float innerCone;
	float outerCone;
	float range;

	SpotLight(glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), float _innerCone = 45.f, float _outerCone = 75.f, float _range = 100.f);

	~SpotLight();

	void Init()override;
	void Update()override;
};