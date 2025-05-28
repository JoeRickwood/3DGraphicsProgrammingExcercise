#pragma once
#include "ObjectInstance.h"
#include "Components.h"

#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

class Scene
{
public:
	static Scene& Current() { static Scene scene; return scene; }

	void AddObject(ObjectInstance* _obj);
	void RemoveObject(ObjectInstance* _obj);
	ObjectInstance* FindObject(std::string _name);

	void Update();
	void Render();
	void ShaderUpdate();

	//Lighting
	void SetPointLight(PointLight* _light, int _index);
	void AddPointLight(PointLight* _light);
	PointLight** GetPointLights();
	int GetPointLightCount();

	void SetSpotLight(SpotLight* _light, int _index);
	void AddSpotLight(SpotLight* _light);
	SpotLight** GetSpotLights();
	int GetSpotLightCount();

	void SetDirectionalLight(DirectionalLight* _directionalLight);
	DirectionalLight* GetDirectionalLight();

	void SetAmbientLightStength(float _strength);
	void SetAmbientLightColor(glm::vec3 _color);
	glm::vec3 GetAmbientLight();

private:
	Scene();
	~Scene();

	std::vector<ObjectInstance*> objects;

	//Point Lights
	PointLight* pointLights[MAX_POINT_LIGHTS];
	int pointLightCount;

	//Directional Light
	DirectionalLight* directionalLight;

	//Spot Lights
	SpotLight* spotLights[MAX_SPOT_LIGHTS];
	int spotLightCount;

	//Ambient Color
	glm::vec3 ambientColor;
	float ambientStrength;
};

