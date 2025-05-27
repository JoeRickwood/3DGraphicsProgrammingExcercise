#pragma once
#include "ObjectInstance.h"
#include "Components.h"
#include "Skybox.h"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

struct PointLight 
{
	glm::vec3 position;
	glm::vec3 color;
	float specularStrength;

	float attenuationConstant;
	float attenuationLinear;
	float attenuationExponent;

	PointLight(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _color = glm::vec3(0.0f, 0.0f, 0.0f), float _specularStrength = 0.0f,
	float _attenuationConstant = 1.0f, float _attenuationLinear = 0.045f, float _attenuationExponent = 0.0075f)
	{
		position = _position;
		color = _color;
		specularStrength = _specularStrength;

		attenuationConstant = _attenuationConstant;
		attenuationLinear = _attenuationLinear;
		attenuationExponent = _attenuationExponent;
	}

	~PointLight() 
	{

	}
};

struct DirectionalLight 
{
	glm::vec3 direction;
	glm::vec3 color;
	float specularStrength;

	DirectionalLight(glm::vec3 _direction = glm::vec3(0.3f, -0.6f, 0.3f), glm::vec3 _color = glm::vec3(1.f, 0.95f, 0.3f), float _specularStrength = 1.f)
	{
		direction = _direction;
		color = _color;
		specularStrength = _specularStrength;
	}

	~DirectionalLight() 
	{

	}
};

struct SpotLight 
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;

	float innerCone;
	float outerCone;

	SpotLight(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _direction = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f), float _innerCone = 45.f, float _outerCone = 75.f)
	{
		position = _position;
		direction = _direction;
		color = _color;

		innerCone = _innerCone;
		outerCone = _outerCone;
	}

	~SpotLight() 
	{

	}
};

class Scene
{
public:
	static Scene& Current() { static Scene scene; return scene; }

	Scene();
	~Scene();

	void AddObject(ObjectInstance* _obj);
	void RemoveObject(ObjectInstance* _obj);
	ObjectInstance* FindObject(std::string _name);

	void Update();
	void Render();
	void ShaderUpdate();


	//Lighting
	void SetPointLight(PointLight _light, int _index);
	void AddPointLight(PointLight _light);
	PointLight* GetPointLights();
	int GetPointLightCount();

	void SetSpotLight(SpotLight _light, int _index);
	void AddSpotLight(SpotLight _light);
	SpotLight* GetSpotLights();
	int GetSpotLightCount();

	void SetDirectionalLight(DirectionalLight _directionalLight);
	DirectionalLight GetDirectionalLight();

private:
	std::vector<ObjectInstance*> objects;

	//Point Lights
	PointLight pointLights[MAX_POINT_LIGHTS];
	int pointLightCount;

	//Directional Light
	DirectionalLight directionalLight;

	//Spot Lights
	SpotLight spotLights[MAX_SPOT_LIGHTS];
	int spotLightCount;
};

