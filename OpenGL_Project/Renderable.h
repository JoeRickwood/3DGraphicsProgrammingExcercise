#pragma once
#include "ShaderLoader.h"
#include "InputManager.h"

class RenderableInstance;

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

	Renderable();
	Renderable(std::vector<GLfloat> _data, std::vector<GLuint> _indices);
	~Renderable();

	void Init();
	void Draw();

	friend class RenderableInstance;
};

