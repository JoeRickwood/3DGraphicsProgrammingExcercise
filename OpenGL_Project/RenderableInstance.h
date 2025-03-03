#pragma once
#include "Renderable.h"

class RenderableInstance;
class Component;

enum RenderableType : int
{
	Triangle = 0,
	Quad = 1
};

class RenderableInstance
{
public:
	Renderable renderable;
	
	glm::vec3 position;
	float rotation;
	glm::vec3 scale;

	//Components Change Parts About The Instance, Position, Rotation Scale, Ect
	std::vector<std::shared_ptr<Component>> components;

	RenderableInstance(RenderableType _type = RenderableType::Quad, glm::vec3 _position = glm::vec3(1.0f, 1.0f, 1.0f), float _rotation = 45.0f, glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f));
	~RenderableInstance();

	void Init();
	void Render();
	void Update();

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


	template <typename T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		T* a = new T(std::forward<Args>(args)...);

		a->parent = this;

		components.emplace_back(a);

		return a;
	}

private:
	glm::mat4 translationMat;
	glm::mat4 rotationMat;
	glm::mat4 scaleMat;
	glm::mat4 modelMat;

	void InitComponents();
	void UpdateComponents();
};

class Component
{
public:
	RenderableInstance* parent;
	Component();
	~Component();

	//Used To Assign Values After Its Parent Has Been Added
	virtual void Init();

	//Called Every Frame
	virtual void Update();
};


