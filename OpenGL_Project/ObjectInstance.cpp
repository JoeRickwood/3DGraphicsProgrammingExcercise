#include "ObjectInstance.h"
#include "Scene.h"

ObjectInstance::ObjectInstance(std::string _name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
{
	name = _name;

	position = _position;
	rotation = _rotation;
	scale = _scale;
}

ObjectInstance::~ObjectInstance()
{
	Scene::Current().RemoveObject(this);
}

void ObjectInstance::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

void ObjectInstance::ShaderUpdate()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->ShaderUpdate();
	}
}

void ObjectInstance::Render()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Render();
	}
}




//COMPONENTS
Component::Component()
{
	enabled = true;
	parent = nullptr;
}

Component::~Component()
{
}

void Component::Init()
{
}

void Component::Update()
{
}

void Component::ShaderUpdate()
{
}

void Component::Render()
{
}

void Component::SetEnabledState(bool state)
{
	enabled = state;
}
