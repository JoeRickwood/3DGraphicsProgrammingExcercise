#pragma once
#include "Camera.h"

struct VertexStandard
{
public:
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;

	VertexStandard() 
	{
		position = glm::vec3();
		texcoord = glm::vec2();
		normal = glm::vec3();
	};

	VertexStandard(glm::vec3 _position, glm::vec2 _texcoord, glm::vec3 _normal) 
	{
		position = _position;
		texcoord = _texcoord;
		normal = _normal;
	};
};

class Renderer;

class Mesh
{
private: //Renderables Contain The Base Data To Be Drawn to The Screen, Used As Basically A "Stamp" Which Multiple Instances Can Use
	bool initialized;

public:
	std::vector<VertexStandard> data;

	GLuint VAO;

	Mesh();
	Mesh(std::vector<VertexStandard> _data);
	~Mesh();

	void Init();

	friend class Renderer;
};

