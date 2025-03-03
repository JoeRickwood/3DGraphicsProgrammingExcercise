#pragma once
#include "ShaderLoader.h"

class Renderable
{
private:
	std::vector<GLfloat> data = {
		//Position          // Color
		-0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f
	};

	std::vector<GLuint> indices = {
		0, 1, 2, //First Triangle
		0, 2, 3  //Second Triangle
	};
	

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

public:
	glm::mat4 translationMat;
	glm::vec3 worldPosition = glm::vec3(0.1f, 0.1f, 0.0f);

	glm::mat4 rotationMat;
	float rotationAngle = 45;

	glm::mat4 scaleMat;
	glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f);

	glm::mat4 modelMat;


	Renderable();
	Renderable(std::vector<GLfloat> _data, std::vector<GLuint> _indices);
	~Renderable();

	void Init();
	void Draw();
};

