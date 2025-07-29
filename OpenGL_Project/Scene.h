#pragma once
#include "UIObjectInstance.h"
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

	//Lighting
	void SetPointLight(PointLight* _light, int _index);
	void AddPointLight(PointLight* _light);
	PointLight** GetPointLights();
	const int GetPointLightCount();

	void SetSpotLight(SpotLight* _light, int _index);
	void AddSpotLight(SpotLight* _light);
	SpotLight** GetSpotLights();
	const int GetSpotLightCount();

	std::vector<ObjectInstance*> GetAllObjects();

	void SetDirectionalLight(DirectionalLight* _directionalLight);
	DirectionalLight* GetDirectionalLight();

	void SetAmbientLightStength(float _strength);
	void SetAmbientLightColor(glm::vec3 _color);
	const glm::vec3 GetAmbientLight();

	int GetCurrentScene() const;
	void ChangeScene(int _scene);
	

protected:
	Scene();
	~Scene();

	std::map<int, std::vector<ObjectInstance*>> objects;

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


	int currentScene;
};

