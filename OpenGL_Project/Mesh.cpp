#include "Mesh.h"

Mesh::Mesh()
{
	initialized = false;

	VAO = NULL;
}

Mesh::Mesh(std::vector<VertexStandard> _data)
{
	data = _data;

	VAO = NULL;

	initialized = false;
}


Mesh::~Mesh()
{
}

void Mesh::Init()
{
	if (initialized) 
	{
		return;
	}

	initialized = true;

	//Generate The VAO 
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate The VBO 
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(VertexStandard), data.data(), GL_STATIC_DRAW);

	//Set Vertex Attribute Info, This Is How To Interpret The Vertex Data
	glVertexAttribPointer(
		0, 
		3, 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(VertexStandard),
		(GLvoid*)(offsetof(VertexStandard, VertexStandard::position)));

	glEnableVertexAttribArray(0);


	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexStandard),
		(GLvoid*)(offsetof(VertexStandard, VertexStandard::texcoord)));

	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexStandard),
		(GLvoid*)(offsetof(VertexStandard, VertexStandard::normal)));

	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
