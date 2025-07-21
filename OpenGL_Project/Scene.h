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

	std::vector<ObjectInstance*> GetAllObjects();

	void SetAmbientLightStength(float _strength);
	void SetAmbientLightColor(glm::vec3 _color);
	const glm::vec3 GetAmbientLight();

protected:
	Scene();
	~Scene();

	std::vector<ObjectInstance*> objects;

	//Ambient Color
	glm::vec3 ambientColor;
	float ambientStrength;
};

