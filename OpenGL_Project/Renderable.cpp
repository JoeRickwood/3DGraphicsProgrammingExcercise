#include "Renderable.h"

Renderable::Renderable()
{
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

	worldPosition = glm::vec3(0.5f, 0.5f, 0.0f);
	rotationAngle = 45.f;
	scale = glm::vec3(0.5f, 0.5f, 1.0f);

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
}


Renderable::~Renderable()
{
}

void Renderable::Init()
{
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void Renderable::Draw()
{
	//Set The New Shader Program
	glUseProgram(ShaderLoader::Instance().GetShaderProgram(0));

	GLint ModelMatLoc = glGetUniformLocation(ShaderLoader::Instance().GetShaderProgram(0), "ModelMatrix");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));

	//Draw Renderable
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	//Unbind The Shader Program
	glUseProgram(0);
}
