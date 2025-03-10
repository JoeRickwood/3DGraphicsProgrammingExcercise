#include "ObjectInstance.h"

//template <typename... Args>
ObjectInstance::ObjectInstance(std::string _name/*, Args&&... args */)
{
	name = _name;

	position = glm::vec3(0.f, 0.f, 0.f);
	rotation = glm::vec3(0.f, 0.f, 0.f);
	scale = glm::vec3(1.f, 1.f, 1.f);

	/*for (auto t : args)
	{	
		AddComponent<Args>(t);
	}  */
}

ObjectInstance::~ObjectInstance()
{
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