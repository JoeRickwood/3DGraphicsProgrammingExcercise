#include "ObjectInstance.h"
#include "Scene.h"

ObjectInstance::ObjectInstance(std::string _name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale)
{
	name = _name;

	position = _position;
	rotation = _rotation;
	scale = _scale;

	parent = nullptr;

	Scene::Current().AddObject(this);
}

ObjectInstance::~ObjectInstance()
{
	Scene::Current().RemoveObject(this);
}

const glm::vec3 ObjectInstance::GetPosition()
{
	if (parent != nullptr) 
	{
		return parent->GetPosition() + position;
	}
	else 
	{
		return position;
	}
}

void ObjectInstance::SetPosition(glm::vec3 _position)
{
	position = _position;
}

const glm::vec3 ObjectInstance::GetRotation()
{
	if (parent != nullptr)
	{
		return parent->GetRotation() + rotation;
	}
	else
	{
		return rotation;
	}
}

void ObjectInstance::SetRotation(glm::vec3 _rotation)
{
	rotation = _rotation;
}

const glm::vec3 ObjectInstance::GetScale()
{
	if (parent != nullptr)
	{
		return parent->GetScale() + scale;
	}
	else
	{
		return scale;
	}
}

void ObjectInstance::SetScale(glm::vec3 _scale)
{
	scale = _scale;
}

const std::string ObjectInstance::GetName()
{
	return name;
}

void ObjectInstance::SetName(std::string _name)
{
	name = _name;
}

const ObjectInstance* ObjectInstance::GetParent()
{
	return parent;
}

void ObjectInstance::SetParent(ObjectInstance* _parent)
{
	parent = _parent;
}

void ObjectInstance::Update()
{
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->Update();
	}
}

void ObjectInstance::ShaderUpdate()
{
	for (int i = 0; i < components.size(); ++i)
	{
		components[i]->ShaderUpdate();
	}
}

void ObjectInstance::Init() 
{
	Scene::Current().AddObject(this);
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

void Component::SetEnabledState(bool state)
{
	enabled = state;
}
