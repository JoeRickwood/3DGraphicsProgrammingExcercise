#include "Lights.h"
#include "Scene.h"
#include <iostream>


// POINT LIGHT

PointLight::PointLight(glm::vec3 _color, float _specularStrength, float _attenuationConstant, float _attenuationLinear, float _attenuationExponent)
{
	color = _color;
	specularStrength = _specularStrength;

	attenuationConstant = _attenuationConstant;
	attenuationLinear = _attenuationLinear;
	attenuationExponent = _attenuationExponent;
}

PointLight::~PointLight()
{
	
}

void PointLight::Init() 
{
	Scene::Current().AddPointLight(this);
}


// DIRECTIONAL LIGHT

DirectionalLight::DirectionalLight(glm::vec3 _direction, glm::vec3 _color, float _specularStrength)
{
	direction = _direction;
	color = _color;
	specularStrength = _specularStrength;
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::Init() 
{
	Scene::Current().SetDirectionalLight(this);
}

// SPOT LIGHT

SpotLight::SpotLight(glm::vec3 _color, float _innerCone, float _outerCone, float _range)
{
	color = _color;

	innerCone = _innerCone;
	outerCone = _outerCone;

	range = _range;

	direction = glm::vec3(0.f, 0.f, 1.0f);
}

SpotLight::~SpotLight()
{
}

void SpotLight::Init() 
{
	Scene::Current().AddSpotLight(this);
}

void SpotLight::Update() 
{
	glm::vec3 tempDir(
		cos(parent->rotation.x) * sin(parent->rotation.y),
		-sin(parent->rotation.x),
		cos(parent->rotation.x) * cos(parent->rotation.y)
	);

	direction = glm::normalize(-tempDir);
}
