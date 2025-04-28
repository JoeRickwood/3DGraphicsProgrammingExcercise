#pragma once
#include "Camera.h"

struct VertexStandard
{
public:
	glm::vec3 position;
	glm::vec2 texcoord;

	VertexStandard() 
	{
		position = glm::vec3();
		texcoord = glm::vec2();
	};

	VertexStandard(glm::vec3 _position, glm::vec2 _texcoord) 
	{
		position = _position;
		texcoord = _texcoord;
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

