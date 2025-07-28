#pragma once
#include "UIObjectInstance.h"
#include "Components.h"

class Scene
{
protected:
	Scene();
	~Scene();

	std::vector<ObjectInstance*> objects;

	//Ambient Color
	glm::vec3 ambientColor;
	float ambientStrength;

public:
	static Scene& Current() { static Scene scene; return scene; }

	void AddObject(ObjectInstance* _obj);
	void RemoveObject(ObjectInstance* _obj);
	ObjectInstance* FindObject(std::string _name);

	void Update();

	std::vector<ObjectInstance*> GetAllObjects();

	void SetAmbientLightStength(float _strength);
	void SetAmbientLightColor(glm::vec3 _color);
	glm::vec3 GetAmbientLight() const;
};

