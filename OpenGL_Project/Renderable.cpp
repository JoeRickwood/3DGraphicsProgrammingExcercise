#include "Renderable.h"

Renderable::Renderable()
{
	initialized = false;

	data = {
		//Position          // Color
		-0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f
	};

	indices = {
		0, 1, 2, //First Triangle
		1, 2, 3  //Second Triangle
	};

	VAO = NULL;
	VBO = NULL;
	EBO = NULL;
}

Renderable::Renderable(std::vector<GLfloat> _data, std::vector<GLuint> _indices)
{
	data = _data;
	indices = _indices;

	VAO = NULL;
	VBO = NULL;
	EBO = NULL;

	initialized = false;
}


Renderable::~Renderable()
{
}

void Renderable::Init()
{
	if (initialized) 
	{
		return;
	}

	initialized = true;

	//Generate The VAO 
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate The EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);

	//Generate The VBO 
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), &data.front(), GL_STATIC_DRAW);

	//Set Vertex Attribute Info, This Is How To Interpret The Vertex Data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);	
}

void Renderable::Draw()
{

}
