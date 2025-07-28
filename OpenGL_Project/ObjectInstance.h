#pragma once
#include "Camera.h"

class Component;

enum ScreenAlignmentX
{
	LEFT,
	MIDDLE,
	Right
};

enum ScreenAlignmentY 
{
	BOTTOM,
	CENTER,
	TOP
};

class ObjectInstance
{
protected:
	ObjectInstance* parent;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::string name;

	std::vector<std::shared_ptr<Component>> components;

public:
	ObjectInstance(std::string _name, glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _rotation = glm::vec3(0.0f), glm::vec3 _scale = glm::vec3(1.f));
	~ObjectInstance();

	//Getters And Setters For Position
	virtual glm::vec3 GetPosition() const;
	void SetPosition(glm::vec3 _position);

	//Getters And Setters For Rotation
	virtual glm::vec3 GetRotation() const;
	void SetRotation(glm::vec3 _rotation);

	//Getters And Setters For Scale
	virtual glm::vec3 GetScale() const;
	virtual glm::vec3 GetLocalScale() const;

	void SetScale(glm::vec3 _scale);


	//Getters And Setters For Name
	virtual std::string GetName() const;
	void SetName(std::string _name);

	//Getters And Setters For Parent
	virtual ObjectInstance* GetParent() const;
	void SetParent(ObjectInstance* _parent, bool resetTransforms = true);

	//Gets The Component Of Type T And Returns The Pointer To It
	template<typename T>
	T* GetComponent()
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
