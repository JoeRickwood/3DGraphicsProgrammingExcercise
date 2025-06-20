#pragma once
#include "Camera.h"

class Component;

class ObjectInstance
{
public:
	std::string name;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	ObjectInstance(std::string _name, glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _rotation = glm::vec3(0.0f), glm::vec3 _scale = glm::vec3(1.f));

	~ObjectInstance();


	//Gets The Component Of Type T And Returns The Pointer To It
	template<typename T>
	T *GetComponent()
	{
		for (const auto& component : components)
		{
			T* casted = dynamic_cast<T*>(component.get());

			if (casted) {
				return casted;
			}
		}

		return nullptr;
	}

	//Adds A Custom Component Of Type T To The Object, Then Returns The Pointer To It
	template <typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		T* a = new T(std::forward<Args>(args)...);

		a->parent = this;

		components.emplace_back(a);

		a->Init();

		return a;
	}

	void Init();
	void Update();
	void ShaderUpdate();
	void Render();

private:
	std::vector<std::shared_ptr<Component>> components;
};

class Component
{
protected:
	bool enabled;

public:
	ObjectInstance* parent; //Parent Gets Set In The Instance's AddComponent Method And Can Be Acessed In Order To Change Values On The Parent

	Component();
	~Component();

	//Used To Assign Values After Its Parent Has Been Added
	virtual void Init();

	//Called Every Frame
	virtual void Update();

	//Called When Shaders Are Assigned To Object, Same Time Model Matrix Is Inputted
	virtual void ShaderUpdate();

	//Called To Enable Or Disable A Component
	void SetEnabledState(bool state);
};

