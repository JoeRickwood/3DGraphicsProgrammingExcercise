#include "ObjectInstance.h"
#include "Scene.h"
#include <iostream>

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

glm::vec3 ObjectInstance::GetPosition() const
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

glm::vec3 ObjectInstance::GetRotation() const
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

glm::vec3 ObjectInstance::GetScale() const
{
	if (parent != nullptr)
	{
		return parent->GetScale() * scale;
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

std::string ObjectInstance::GetName() const
{
	return name;
}

void ObjectInstance::SetName(std::string _name)
{
	name = _name;
}

ObjectInstance* ObjectInstance::GetParent() const
{
	return parent;
}

void ObjectInstance::SetParent(ObjectInstance* _parent, bool resetTransforms)
{
	if (_parent == this) 
	{
		parent = nullptr;
		return;
	}

	if (resetTransforms) 
	{
		position = position - _parent->position;
		rotation = rotation - _parent->rotation;
		scale = scale / _parent->scale;
	}

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
