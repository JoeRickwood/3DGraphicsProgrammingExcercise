#include "Scene.h"
#include "Time.h"

Scene::Scene()
{
	pointLightCount = 0;
	spotLightCount = 0;
}

Scene::~Scene()
{
}

void Scene::AddObject(ObjectInstance* _obj)
{
	objects.push_back(_obj);
}

void Scene::RemoveObject(ObjectInstance* _obj)
{
	for (int i = 0; i < objects.size(); ++i)
	{
		if (objects[i] == _obj)
		{
			objects.erase(objects.begin() + i);
			return;
		}
	}
}

void Scene::SetPointLight(PointLight _light, int _index)
{
	//Does Not Set The Light If Index Does Not Exist
	if (_index < 0 || _index >= MAX_POINT_LIGHTS) 
	{
		return;
	}

	pointLights[_index] = _light;
}

void Scene::AddPointLight(PointLight _light)
{
	//Sets The Point Light Of The Current Index
	pointLights[pointLightCount] = _light;

	//Increments PointLightCount If Its Less Than The Max Light Count
	if (pointLightCount < MAX_POINT_LIGHTS) 
	{
		pointLightCount++;
	}
}

PointLight* Scene::GetPointLights()
{
	//Returns The Current Array Of Point Lights
	return pointLights;
}

int Scene::GetPointLightCount()
{
	return pointLightCount;
}

void Scene::SetSpotLight(SpotLight _light, int _index)
{
	//Does Not Set The Light If Index Does Not Exist
	if (_index < 0 || _index >= MAX_SPOT_LIGHTS)
	{
		return;
	}

	spotLights[_index] = _light;
}

void Scene::AddSpotLight(SpotLight _light)
{
	//Sets The Spot Light Of The Current Index
	spotLights[spotLightCount] = _light;

	//Increments PointLightCount If Its Less Than The Max Light Count
	if (spotLightCount < MAX_POINT_LIGHTS)
	{
		spotLightCount++;
	}
}

SpotLight* Scene::GetSpotLights()
{
	return spotLights;
}

int Scene::GetSpotLightCount()
{
	return spotLightCount;
}

void Scene::SetDirectionalLight(DirectionalLight _directionalLight)
{
	directionalLight = _directionalLight;
}

DirectionalLight Scene::GetDirectionalLight()
{
	return directionalLight;
}

ObjectInstance* Scene::FindObject(std::string _name)
{
	for (auto& obj : objects)
	{
		if (obj->name == _name) 
		{
			return obj;
		}
	}

	return nullptr;
}

void Scene::Update()
{
	for (auto& obj : objects)
	{
		if (obj == nullptr) 
		{
			continue;
		}

		obj->Update();
	}
}

void Scene::Render()
{
	glViewport(0, 0, (GLsizei)GraphicsLoader::Instance().windowSize.x, (GLsizei)GraphicsLoader::Instance().windowSize.y);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->Render();
	}
}

void Scene::ShaderUpdate()
{
	for (int i = 0; i < objects.size(); ++i)
	{
		objects[i]->ShaderUpdate();
	}
}