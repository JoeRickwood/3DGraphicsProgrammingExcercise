#include "Scene.h"
#include "Time.h"

void Scene::SetAmbientLightStength(float _strength)
{
	ambientStrength = _strength;
}

void Scene::SetAmbientLightColor(glm::vec3 _color)
{
	ambientColor = _color;
}

const glm::vec3 Scene::GetAmbientLight()
{
	return ambientColor * ambientStrength;
}

int Scene::GetCurrentScene() const
{
	return currentScene;
}

void Scene::ChangeScene(int _scene)
{
	currentScene = _scene;
}

Scene::Scene()
{
	currentScene = 1;

	pointLightCount = 0;
	spotLightCount = 0;

	ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientStrength = 0.2f;

	directionalLight = nullptr;
}

Scene::~Scene()
{
}

void Scene::AddObject(ObjectInstance* _obj)
{
	if (_obj == nullptr)
	{
		return;
	}

	objects[currentScene].push_back(_obj);
	_obj->SetScene(currentScene);
}

void Scene::RemoveObject(ObjectInstance* _obj)
{
	for (int i = 0; i < objects[currentScene].size(); ++i)
	{
		if (objects[currentScene][i] == _obj)
		{
			objects[currentScene].erase(objects[currentScene].begin() + i);
			return;
		}
	}
}

void Scene::SetPointLight(PointLight* _light, int _index)
{
	//Does Not Set The Light If Index Does Not Exist
	if (_index < 0 || _index >= MAX_POINT_LIGHTS) 
	{
		return;
	}

	pointLights[_index] = _light;
}

void Scene::AddPointLight(PointLight* _light)
{
	//Sets The Point Light Of The Current Index
	pointLights[pointLightCount] = _light;

	//Increments PointLightCount If Its Less Than The Max Light Count
	if (pointLightCount < MAX_POINT_LIGHTS) 
	{
		pointLightCount++;
	}
}

PointLight** Scene::GetPointLights()
{
	//Returns The Current Array Of Point Lights
	return pointLights;
}

const int Scene::GetPointLightCount()
{
	return pointLightCount;
}

void Scene::SetSpotLight(SpotLight* _light, int _index)
{
	//Does Not Set The Light If Index Does Not Exist
	if (_index < 0 || _index >= MAX_SPOT_LIGHTS)
	{
		return;
	}

	spotLights[_index] = _light;
}

void Scene::AddSpotLight(SpotLight* _light)
{
	//Sets The Spot Light Of The Current Index
	spotLights[spotLightCount] = _light;

	//Increments PointLightCount If Its Less Than The Max Light Count
	if (spotLightCount < MAX_POINT_LIGHTS)
	{
		spotLightCount++;
	}
}

SpotLight** Scene::GetSpotLights()
{
	return spotLights;
}

const int Scene::GetSpotLightCount()
{
	return spotLightCount;
}

std::vector<ObjectInstance*> Scene::GetAllObjects()
{
	return objects[currentScene];
}

void Scene::SetDirectionalLight(DirectionalLight* _directionalLight)
{
	directionalLight = _directionalLight;
}

DirectionalLight* Scene::GetDirectionalLight()
{
	return directionalLight;
}

ObjectInstance* Scene::FindObject(std::string _name)
{
	for (auto& obj : objects[currentScene])
	{
		if (obj->GetName() == _name)
		{
			return obj;
		}
	}

	return nullptr;
}

void Scene::Update()
{
	for (auto& obj : objects[currentScene])
	{
		if (obj == nullptr)
		{
			continue;
		}

		obj->Update();
	}
}